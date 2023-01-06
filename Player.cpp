#include "stdafx.h"
#include "Player.h"
#include "Terrain.h"
#include "AStar_Manager.h"


CPlayer::CPlayer()
{

}


CPlayer::~CPlayer()
{
	Release_GameObject(); 
}



void CPlayer::MovePlayer()
{
	list<TILE*>& BestList = CAStar_Manager::Get_Instance()->Get_BestList();
	if (BestList.empty())
	{
		if (CCombat_Manager::Get_Instance()->Get_SelectedObject() != dynamic_cast<CGameObject*>(this))
		{
			Change_AnimState(m_wstrName, L"Idle");
			if(m_bIsMoving)
			{ 
				CKey_Manager::Get_Instance()->Set_KeyLock(false);
				m_bIsMoving = false;
			}
		}
		return;
	}
	m_tInfo.vDir = BestList.front()->vPos - m_tInfo.vPos; 
	float fDist = D3DXVec3Length(&m_tInfo.vDir); 
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir); 
	m_bIsMoving = true;

	if (3.f > fDist)
	{
		//m_dwUnitState |= UNIT_MOVE;
		m_dwLocate = BestList.front()->iIndex;
		m_tInfo.vPos = BestList.front()->vPos;
		m_tUnitInfo.iCurMoveRange -= 1;
	//	printf("%f %f %f\n", m_tInfo.vPos.x, m_tInfo.vPos.y, m_tInfo.vPos.z);
		BestList.pop_front();
	}
	if (m_tInfo.vDir.x < -0.9)
		Change_AnimState(m_wstrName, L"Move_Left");

	if (m_tInfo.vDir.x > 0.9)
		Change_AnimState(m_wstrName, L"Move_Right");

	if (m_tInfo.vDir.y < -0.9)
		Change_AnimState(m_wstrName, L"Move_Back");

	if (m_tInfo.vDir.y > 0.9)
		Change_AnimState(m_wstrName, L"Move_Front");

	m_tInfo.vPos += m_tInfo.vDir * CTime_Manager::Get_Instance()->Get_DeltaTime() * m_fSpeed; 
	if(m_wstrName==L"Paladin")
		CSoundMgr::Get_Instance()->PlaySound(L"Walk0.wav",CSoundMgr::PLAYER);
	else
		CSoundMgr::Get_Instance()->PlaySound(L"Walk1.wav", CSoundMgr::PLAYER);
}



HRESULT CPlayer::Ready_GameObject()
{
	m_fSpeed = 300.f; 

	m_tUnitInfo.iCurMoveRange = m_tUnitInfo.iMaxMoveRange;

	if(m_wstrName==L"")
		m_wstrName = L"Eirik";

	m_pCurAnimClip = CAnimClip_Manager::Get_Instance()->Get_Clip(m_wstrName, L"Idle");

	return S_OK;
}
int CPlayer::Update_GameObject()
{
	if (m_tUnitInfo.iCurHp <= 0)
	{
		
		Set_Locate(OBJ_DEAD_PLACE);
		Set_AddUnitState(UNIT_MOVE | UNIT_ATTACK | UNIT_END);
		return OBJ_NOEVENT;
	}
	return OBJ_NOEVENT; 

}
void CPlayer::Late_Update_GameObject()
{
	PosFrameMove();
	FrameMove(); 
	if(m_bIsMoving)
		MovePlayer();
}

void CPlayer::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_Manager(m_pCurAnimClip->wstrObjectKey, m_pCurAnimClip->wstrStateKey, (DWORD)m_pCurAnimClip->fFrameStart);
	if (nullptr == pTexInfo)
		return;
	/*const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_Manager(m_tCurAnimClip.wstrObjectKey, m_tCurAnimClip.wstrStateKey, (DWORD)m_tCurAnimClip.fFrameStart);
	if (nullptr == pTexInfo)
		return; */
	D3DXMATRIX matScale, matTrans, matWorld; 
	if (m_pCurAnimClip->wstrStateKey == L"Move_Right")
	{
		D3DXMatrixScaling(&matScale, -m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	}
	else D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f); 
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + CScroll_Manager::Get_Instance()->Get_Scroll(CScroll_Manager::eSB_HORZ), m_tInfo.vPos.y + CScroll_Manager::Get_Instance()->Get_Scroll(CScroll_Manager::eSB_VERT), 0.f);
	matWorld = matScale * matTrans; 
	float fCenterX = pTexInfo->tImageInfo.Width >> 1; 
	float fCenterY = pTexInfo->tImageInfo.Height >> 1; 

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld); 
	if (m_dwUnitState & UNIT_MOVE && m_dwUnitState & UNIT_ATTACK)
	{
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 80, 80, 80));

	}
	else
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255)); 
}

void CPlayer::Release_GameObject()
{
}

CGameObject* CPlayer::Create(LPVOID* pParam)
{
	CGameObject* pInstance = new CPlayer;
	if (0 > pInstance->Ready_GameObject())
	{
		Safe_Delete(pInstance);
		return pInstance;
	}
	if (nullptr != pParam)
		dynamic_cast<CPlayer*>(pInstance)->Set_DESC((UNIT_DESC*)pParam);

	return pInstance;
}