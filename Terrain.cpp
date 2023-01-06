#include "stdafx.h"
#include "Terrain.h"

CTerrain::CTerrain()
	:m_PlayerLocateList({})
	, m_EnemyLocateList({})
{
}


CTerrain::~CTerrain()
{
	Release_GameObject(); 
}

int CTerrain::Get_TileIndex(const D3DXVECTOR3& vPos)
{
	int X = ((vPos.x + (TILECX / 2)) / (TILECX));
	int Y = ((vPos.y + (TILECY / 2)) / (TILECY));

	int iIndex = Y * TILEX + X;

	if (0 > iIndex)
		return -1;
	return iIndex;
}


HRESULT CTerrain::Create_Graph_Terrain()
{
	m_vecGraph.resize(TILEX * TILEY); 

	for (int i = 0 ; i < TILEY; ++i)
	{
		for (int j = 0 ; j < TILEX ; ++j)
		{
			int iIndex = j + (i * TILEX); 
			// Top
			if (0 != i && iIndex != 0)
			{
				if (0 != m_vecTile[iIndex - TILEX]->byOption)
					m_vecGraph[iIndex].emplace_back(m_vecTile[iIndex - TILEX]);
			}
			// LEFT
			if (0 != j)
			{
				if (0 != m_vecTile[iIndex - 1]->byOption)
					m_vecGraph[iIndex].emplace_back(m_vecTile[iIndex - 1]);
			}
			
			// Bottom
			if (TILEY-1 != i )
			{
				if (0 != m_vecTile[iIndex + TILEX]->byOption)
					m_vecGraph[iIndex].emplace_back(m_vecTile[iIndex + TILEX]);
			}

			// RIGHT
			if (TILEX-1 != j)
			{
				if (0 != m_vecTile[iIndex + 1]->byOption)
					m_vecGraph[iIndex].emplace_back(m_vecTile[iIndex + 1 ]);
			}

		}
	}
	return S_OK;
}

HRESULT CTerrain::LoadTileData_Terrain(const wstring & wstrFilePath)
{
	HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr); 
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL; 

	DWORD dwbyte = 0; 

	DWORD dwStageNumber = 0;
	ReadFile(hFile, &dwStageNumber, sizeof(DWORD), &dwbyte, nullptr);
	m_dwStageNumber = dwStageNumber;

	TILE* pTile = nullptr; 
	while (true)
	{
		pTile = new TILE; 
		ReadFile(hFile, pTile, sizeof(TILE), &dwbyte, nullptr); 
		if (0 == dwbyte)
		{
			Safe_Delete(pTile); 
			break; 
		}
		pTile->bHighLightMoveRange = false;
		pTile->bHighLightAttackRange = false;
		pTile->bOnUnit = false;
		m_vecTile.emplace_back(pTile); 
	}
	CloseHandle(hFile); 

	Create_Graph_Terrain();

	return S_OK;
}

// 하이라이트 기능도 같이 있음
list<int>* CTerrain::Get_AttackAbleList(CGameObject* pObject)
{
	m_AttackAbleIndexList.clear();

	auto UnitInfo = pObject->Get_UnitInfo();
	
	int iAttackRange = UnitInfo.iAttackRange;
	int iIndex = pObject->Get_Locate();

	/*m_vecTile[iIndex]->bHighLightAttackRange = true;
	m_MinDist[iIndex] = 0;*/

	HighlightTile(iIndex, 0, iAttackRange);

	return &m_AttackAbleIndexList;
}

void CTerrain::HighlightTile(int iIndex, int iMoveRange, int iAttackRange)
{
	int totalRange = iMoveRange + iAttackRange;

	int iSize = sizeof(m_MinDist) / sizeof(m_MinDist[0]);
	
	CalculateRange(iIndex, totalRange, 0);
	
	for (int i = 0; i < iSize; i++)
	{
		if (m_MinDist[i] != -1)
		{
			if (m_MinDist[i] <= iMoveRange)
			{
				m_SelectableIndexList.push_back(i);
				m_AttackAbleIndexList.push_back(i);
				m_vecTile[i]->bHighLightMoveRange = true;
				m_vecTile[i]->bHighLightAttackRange = true;
			}
			else 
			{
				m_AttackAbleIndexList.push_back(i);
				m_vecTile[i]->bHighLightAttackRange = true;
			}
		}
		m_MinDist[i] = -1;
	}
}

void CTerrain::DisableHighlightAndClear()
{
	
	for each (int it in m_SelectableIndexList)
	{
		m_vecTile[it]->bHighLightMoveRange = false;
		m_vecTile[it]->bHighLightAttackRange = false;
	}
	for each (int it in m_AttackAbleIndexList)
	{
		m_vecTile[it]->bHighLightMoveRange = false;
		m_vecTile[it]->bHighLightAttackRange = false;
	}
	for each (int it in m_DontMoveIndexList)
	{
		m_vecTile[it]->bHighLightMoveRange = false;
		m_vecTile[it]->bHighLightAttackRange = false;
		m_vecTile[it]->bOnUnit = false;
	}

	while (!m_SelectableIndexList.empty())
	{
		m_SelectableIndexList.pop_back();
	}
	m_SelectableIndexList.clear();
	m_AttackAbleIndexList.clear();
	m_DontMoveIndexList.clear();
}

void CTerrain::CheckUnitOnTerrain()
{
	while (!m_PlayerLocateList.empty())
		m_PlayerLocateList.pop_back();
	while (!m_EnemyLocateList.empty())
	{
		m_vecTile[m_EnemyLocateList.back()]->bOnUnit = false;
		m_EnemyLocateList.pop_back();
	}
	m_PlayerLocateList.clear();
	m_EnemyLocateList.clear();
	for each (CGameObject* pObject in CGameObject_Manager::Get_Instance()->Get_List(PLAYER))
	{
		m_vecTile[pObject->Get_Locate()]->bOnUnit = true;
		m_PlayerLocateList.push_back(pObject->Get_Locate());
		//m_ChangedIndexList.push_back(pObject->Get_Locate());
	}
	for each (CGameObject * pObject in CGameObject_Manager::Get_Instance()->Get_List(ENEMY))
	{
		m_vecTile[pObject->Get_Locate()]->bOnUnit = true;
		m_EnemyLocateList.push_back(pObject->Get_Locate());
		//m_ChangedIndexList.push_back(pObject->Get_Locate());
	}
}

void CTerrain::Update_Graph_Terrain()
{

}


void CTerrain::ClearTileState()
{
}

void CTerrain::CalculateRange(int CurIndex, int MaxDist, int CurDist)
{

	if (CurDist > MaxDist)
		return;
	
	m_MinDist[CurIndex] = CurDist;

	int iTileX = static_cast<int>(TILEX);
	int dX[4] = { 0,	-1,		0,	 1 };
	int dY[4] = { iTileX,  0,  -iTileX, 0 };


	for (int i = 0; i < 4; i++)
	{
		int NextIndex = CurIndex + dX[i] + dY[i];

		if (NextIndex >= 0 && NextIndex < (TILEX * TILEY))
		{
			if (m_MinDist[NextIndex] != DontMoveTile)
			{
				if (m_vecTile[NextIndex]->byOption != 0)
				{
					if (m_MinDist[NextIndex] > CurDist || m_MinDist[NextIndex] == -1)
					{
						
						CalculateRange(NextIndex, MaxDist, CurDist + 1);
					}
				}
				if (m_vecTile[NextIndex]->byOption == 0)
				{
					if (m_MinDist[NextIndex] < CurDist && m_MinDist[NextIndex] != -1)
						break;

					m_vecTile[NextIndex]->bHighLightMoveRange = true;
					m_MinDist[NextIndex] = DontMoveTile;
					m_DontMoveIndexList.push_back(NextIndex);
				}
			}
		}
	}
}


HRESULT CTerrain::Ready_GameObject()
{
	m_dwStageNumber = 0;

	int iSize = sizeof(m_MinDist) / sizeof(m_MinDist[0]);
	for (int i = 0; i < iSize; i++)
		m_MinDist[i] = -1;

	//LoadTileData_Terrain(L"../Data/StageA.dat"); 
	//Create_Graph_Terrain();
	return S_OK;
}


int CTerrain::Update_GameObject()
{
	float fSpeed = 300.f * CTime_Manager::Get_Instance()->Get_DeltaTime(); 

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		CScroll_Manager::Get_Instance()->Set_Scroll({ fSpeed, 0.f, 0.f });
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		CScroll_Manager::Get_Instance()->Set_Scroll({ -fSpeed, 0.f, 0.f });
	if (GetAsyncKeyState(VK_UP) & 0x8000)
		CScroll_Manager::Get_Instance()->Set_Scroll({ 0.f,fSpeed,  0.f });
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		CScroll_Manager::Get_Instance()->Set_Scroll({ 0.f,-fSpeed,  0.f });

	return 0;
}

void CTerrain::Late_Update_GameObject()
{
}

void CTerrain::Render_GameObject()
{
	D3DXMATRIX matScale, matTrans, matWorld;

	D3DXMatrixScaling(&matScale, 4.0f, 4.0f, 0.f);
	D3DXMatrixTranslation(&matTrans, CScroll_Manager::Get_Instance()->Get_Scroll(CScroll_Manager::eSB_HORZ), CScroll_Manager::Get_Instance()->Get_Scroll(CScroll_Manager::eSB_VERT), 0.f);
	matWorld = matScale * matTrans;
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_Manager(L"Terrain", L"LocalMap", m_dwStageNumber);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);// 배경은 -8.f 만큼 보정해줌
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(-8.f, -8.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	DWORD dwSize = m_vecTile.size();
	DWORD dwDrawID = 0;
	m_bIsDrawId = false;
	for (size_t i = 0; i < dwSize; ++i)
	{
		if (m_vecTile[i]->bOnUnit)
		{
			dwDrawID = 4;
		/*	if (m_vecTile[i]->bHighLightAttackRange)
				dwDrawID = 5;*/
			m_bIsDrawId = true;
		}
		else if (m_vecTile[i]->bHighLightMoveRange)
		{
			if (m_vecTile[i]->byOption != 0)
				dwDrawID = 8;
			else
				dwDrawID = 0;
			m_bIsDrawId = true;
		}
		else if (m_vecTile[i]->bHighLightAttackRange)
		{
			dwDrawID = 5;
			m_bIsDrawId = true;
		}
		else {
			/*const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_Manager(L"Terrain", L"Tile", m_vecTile[i]->byDrawID);

			TerrainTileRender(pTexInfo, i);*/
		}
		if (m_bIsDrawId)
		{
			const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_Manager(L"Terrain", L"Tile", dwDrawID);

			D3DXMATRIX matScale, matTrans, matWorld;

			D3DXMatrixScaling(&matScale, 4.0f, 4.0f, 0.f);
			D3DXMatrixTranslation(&matTrans, CScroll_Manager::Get_Instance()->Get_Scroll(CScroll_Manager::eSB_HORZ), CScroll_Manager::Get_Instance()->Get_Scroll(CScroll_Manager::eSB_VERT), 0.f);
			matWorld = matScale * matTrans;

			if (nullptr == pTexInfo)
				return;
			float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
			float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

			D3DXMatrixScaling(&matScale, m_vecTile[i]->vSize.x, m_vecTile[i]->vSize.y, 0.f);
			D3DXMatrixTranslation(&matTrans, m_vecTile[i]->vPos.x + CScroll_Manager::Get_Instance()->Get_Scroll(CScroll_Manager::eSB_HORZ), m_vecTile[i]->vPos.y + CScroll_Manager::Get_Instance()->Get_Scroll(CScroll_Manager::eSB_VERT), 0.f);
			matWorld = matScale * matTrans;

			CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
			CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(200, 255, 255, 255));
		}
		m_bIsDrawId = false;
	}
	
}

void CTerrain::Release_GameObject()
{
	for (auto& pTile : m_vecTile)
		Safe_Delete(pTile);
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();

	while (!m_SelectableIndexList.empty())
		m_SelectableIndexList.pop_back();
	m_SelectableIndexList.clear();
	
	while (!m_DontMoveIndexList.empty())
		m_DontMoveIndexList.pop_back();
	m_DontMoveIndexList.clear();

	while (!m_EnemyLocateList.empty())
		m_EnemyLocateList.pop_back();
	m_EnemyLocateList.clear();

	while (!m_PlayerLocateList.empty())
		m_PlayerLocateList.pop_back();
	m_PlayerLocateList.clear();

	int iSize = m_vecGraph.size();
	for (int i = iSize-1; i > 0; i--)
	{
		while (!m_vecGraph[i].empty())
			m_vecGraph[i].pop_back();
		m_vecGraph.pop_back();
	}
}
