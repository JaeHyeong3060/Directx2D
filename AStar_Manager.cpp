#include "stdafx.h"
#include "AStar_Manager.h"
#include "Terrain.h"
IMPLEMENT_SINGLETON(CAStar_Manager)
CAStar_Manager::CAStar_Manager()
{
}


CAStar_Manager::~CAStar_Manager()
{
}

void CAStar_Manager::StartAStar_Manager(const D3DXVECTOR3 & vStartPos, const D3DXVECTOR3 & vGoalPos)
{
	// 10분뒤에. 
	// 아 이제 에이스타를 돌려야 겠구나~ 라는 생각이 들지. 
	m_OpenList.clear(); 
	m_CloseList.clear();
	// 이건 클리어만 해야한다~? 알겠죠 ??
	m_BestList.clear(); 

	m_iStartIndex = Get_TileIndex2(vStartPos); 
	int iGoalIndex = Get_TileIndex2(vGoalPos); 

	if (-1 == m_iStartIndex || -1 == iGoalIndex)
		return; 
	if (m_iStartIndex == iGoalIndex)
		return; 

	CTerrain*  pTerrain = dynamic_cast<CTerrain*>(CGameObject_Manager::Get_Instance()->Get_Terrain(0));
	auto& vecTile = pTerrain->Get_VecTile();
	//여러개의 옵션값을 줄경우 이렇게 조건을 걸게 되면 뭐한다? 
	//안가. 
	if (0 == vecTile[iGoalIndex]->byOption) // option = 0 이동 불가능 지역
		return; 

	if (PathFinding_AStar_Manager(m_iStartIndex, iGoalIndex))
		Make_Route(iGoalIndex); 
}

bool CAStar_Manager::Check_OpenList(int iIndex)
{
	auto& iter_find = find(m_OpenList.begin(), m_OpenList.end(), iIndex);
	if (iter_find == m_OpenList.end())
		return false; 

	return true;
}

bool CAStar_Manager::Check_CloseList(int iIndex)
{
	auto& iter_find = find(m_CloseList.begin(), m_CloseList.end(), iIndex);
	if (iter_find == m_CloseList.end())
		return false;

	return true;
}

bool CAStar_Manager::PathFinding_AStar_Manager(int iStartIndex, int iGoalIndex)
{
	if (!m_OpenList.empty())
		m_OpenList.pop_front(); 

	m_CloseList.emplace_back(iStartIndex); 
	CTerrain*  pTerrain = dynamic_cast<CTerrain*>(CGameObject_Manager::Get_Instance()->Get_Terrain(0));
	auto& vecTile = pTerrain->Get_VecTile();
	auto& vecGraph = pTerrain->Get_Graph(); 
	for (auto&  pTile : vecGraph[iStartIndex] )
	{
		if (iGoalIndex == pTile->iIndex)
		{
			pTile->iParentIndex = iStartIndex;
			return true;
		}

		if(Check_CloseList(pTile->iIndex) || Check_OpenList(pTile->iIndex))
			continue;
		pTile->iParentIndex = iStartIndex; 
		m_OpenList.emplace_back(pTile->iIndex);
	}
	// 이경우에 걸린다?? 그럼 모든 타일을 전부다 순회했다라는 말. 
	if (m_OpenList.empty())
		return false;

	//greater<int>();
	m_OpenList.sort([&](int iPreIndex, int iNextIndex) 
	{
		D3DXVECTOR3 vPreToStart = vecTile[m_iStartIndex]->vPos - vecTile[iPreIndex]->vPos;
		D3DXVECTOR3 vPreToGoal = vecTile[iGoalIndex]->vPos - vecTile[iPreIndex]->vPos;
		float fDist1 = D3DXVec3Length(&vPreToGoal) + D3DXVec3Length(&vPreToStart); 

		D3DXVECTOR3 vNextToStart = vecTile[m_iStartIndex]->vPos - vecTile[iNextIndex]->vPos;
		D3DXVECTOR3 vNextToGoal = vecTile[iGoalIndex]->vPos - vecTile[iNextIndex]->vPos;
		float fDist2 = D3DXVec3Length(&vNextToStart) + D3DXVec3Length(&vNextToGoal);

		return fDist1 < fDist2; 
	});

	return PathFinding_AStar_Manager(m_OpenList.front(), iGoalIndex);
}

void CAStar_Manager::Make_Route(int iGoalIndex)
{
	CTerrain*  pTerrain = dynamic_cast<CTerrain*>(CGameObject_Manager::Get_Instance()->Get_Terrain(0));
	auto& vecTile = pTerrain->Get_VecTile();
	auto& vecGraph = pTerrain->Get_Graph();

	int iParentIndex = vecTile[iGoalIndex]->iParentIndex; 
	m_BestList.emplace_back(vecTile[iGoalIndex]); 
	while (true)
	{
		if (iParentIndex == m_iStartIndex)
			break; 
		m_BestList.emplace_front(vecTile[iParentIndex]);
		iParentIndex = vecTile[iParentIndex]->iParentIndex; 
	}
	

}

int CAStar_Manager::Get_TileIndex2(const D3DXVECTOR3 & vPos)
{
	CTerrain*  pTerrain = dynamic_cast<CTerrain*>(CGameObject_Manager::Get_Instance()->Get_Terrain(0)); 
	auto& vecTile = pTerrain->Get_VecTile(); 
	int X = ((vPos.x + (TILECX / 2)) / (TILECX));
	int Y = ((vPos.y + (TILECY / 2)) / (TILECY));

	int iIndex = Y * TILEX + X;

	if (-1 == iIndex)
		return -1;
	return iIndex;
}
