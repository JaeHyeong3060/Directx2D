#include "stdafx.h"
#include "Combat_Manager.h"
#include "GameObject.h"
#include "UI_Object.h"
#include "UI_UnitMenu.h"
#include "Terrain.h"
#include "Player.h"
#include "Enemy.h"
#include "MyButton.h"
IMPLEMENT_SINGLETON(CCombat_Manager)

void CCombat_Manager::Ready_Combat_Manager()
{
	m_dwUnitIndex = 0;
	m_dwGameState = 0;
	m_dwCursorState = 0;
	m_dwStageNumber = 0;
	m_pTerrain = dynamic_cast<CTerrain*>(CGameObject_Manager::Get_Instance()->Get_Terrain(0));

	m_pSourceObject = nullptr;

	m_eCurTurn = ePLAYER_TURN;
	m_bIsLoading = true;

	m_bIsMenuUI = false;
	bIsEnemyTurnEnd = false;
	m_IsEnemyAllDead = false;
	m_IsPlayerAllDead = false;

	m_pSelectedObject = nullptr;

	m_AttackableList = {};

	m_dwSelectedMenuIndex = 0;

}

int CCombat_Manager::Update_Combat_Manager()
{
	if (m_bIsLoading == true)
		return 0;

	auto playerList = CGameObject_Manager::Get_Instance()->Get_List(PLAYER);
	auto enemyList = CGameObject_Manager::Get_Instance()->Get_List(ENEMY);

	auto checkPlayerAllDead = find_if(playerList.begin(), playerList.end(), [](CGameObject* pObject) {return pObject->Get_Locate() != OBJ_DEAD_PLACE; });
	if (checkPlayerAllDead == playerList.end())
	{
		printf("플레이어 전부 죽음");
		//TODO
		return EVENT_PLAYER_ALL_DEAD;
	}

	auto checkEnemyAllDead = find_if(enemyList.begin(), enemyList.end(), [](CGameObject* pObject) {return pObject->Get_Locate() != OBJ_DEAD_PLACE; });
	if (checkEnemyAllDead == enemyList.end())
	{
		printf("적 전부 죽음");
		return EVENT_ENEMY_ALL_DEAD;
	}

#pragma region PLAYER_TURN_SYSTEM

	int EndUnitCount = 0;

	if (m_eCurTurn == ePLAYER_TURN)
	{
		if (CKey_Manager::Get_Instance()->Key_Down(KEY_LBUTTON))
		{
			m_pTerrain->CheckUnitOnTerrain(); // 보험 삼아서 여기서 체크 해줌
			D3DXVECTOR3 vScroll = CScroll_Manager::Get_Instance()->Get_Scroll();
			POINT pt{};
			GetCursorPos(&pt);
			ScreenToClient(g_hWND, &pt);
			D3DXVECTOR3 vMouse{ float(pt.x) - vScroll.x, float(pt.y) - vScroll.y , 0.f };
			int iMouseIndex = m_pTerrain->Get_TileIndex(vMouse);

			if (m_dwCursorState & UNIT_SELECT)
			{
				DWORD SelectedObjectState = m_pSelectedObject->Get_UnitState();
				if (!(SelectedObjectState & UNIT_MOVE))
				{
					auto SeletableTerrainIndexList = m_pTerrain->Get_SelectableList();
					auto it = find(SeletableTerrainIndexList->begin(), SeletableTerrainIndexList->end(), iMouseIndex);
					if (it != SeletableTerrainIndexList->end())
					{
						if (false == m_pTerrain->Get_TileOnUnit(iMouseIndex))
						{
							CKey_Manager::Get_Instance()->Set_KeyLock(true);

							m_pTerrain->Set_TileOnUnit(m_pSelectedObject->Get_Locate(), false);
							m_pTerrain->DisableHighlightAndClear();
							m_dwCursorState ^= UNIT_SELECT;
							m_pSelectedObject->Set_AddUnitState(UNIT_MOVE);
							m_pSelectedObject->Set_Moving(true);
							m_dwUnitIndex = -1;
							CAStar_Manager::Get_Instance()->StartAStar_Manager(m_pSelectedObject->Get_Info()->vPos, vMouse);
							m_pSelectedObject->Change_AnimState(m_pSelectedObject->Get_Name(), L"Idle");
							//pTerrain->Set_TileOnUnit(m_pSelectedObject->Get_Locate(), true);



							m_pSelectedObject = nullptr;
						}
					}
					else
					{
						m_pTerrain->DisableHighlightAndClear();
						m_dwCursorState ^= UNIT_SELECT;
						m_dwUnitIndex = -1;
						m_pSelectedObject->Change_AnimState(m_pSelectedObject->Get_Name(), L"Idle");
						m_pSelectedObject = nullptr;
					}
				}
				else if(m_dwSelectedMenuIndex==1 && m_pSelectedObject->Get_UnitState()!= UNIT_END){
					PlayerAttackReady(iMouseIndex);
				}
				else if (m_dwSelectedMenuIndex == 2 && m_pSelectedObject->Get_UnitState() != UNIT_END) {
					//PlayerAttackReady(iMouseIndex);
					//TODO
				}
				else if (m_dwSelectedMenuIndex == 3 && m_pSelectedObject->Get_UnitState() != UNIT_END)
				{
					m_pSelectedObject->Set_AddUnitState(UNIT_ATTACK | UNIT_END);
				}
				//else if (m_bIsMenuUI)
				//{
				//	switch (m_dwSelectedMenuIndex)
				//	{
				//	case 1:
				//	{
				//		
				//		m_bIsMenuUI = false;
				//	}
				//		break;
				//	case 2:
				//		m_bIsMenuUI = false;
				//		break;
				//	case 3:
				//		m_bIsMenuUI = false;
				//		break;
				//	default:

				//		break;
				//	}
				//	//m_dwSelectedMenuIndex = 0;
				//}
			
			}
			if (!(m_dwCursorState & UNIT_SELECT)) {
				for (auto& it : playerList)
				{
					if ((it->Get_Locate() == iMouseIndex) && !(it->Get_UnitState() & UNIT_END) && (m_bIsMenuUI==false))
					{
						m_dwCursorState |= UNIT_SELECT;
						m_dwUnitIndex = iMouseIndex;
						it->Set_Selected(true);
						m_pSelectedObject = it;
						it->Change_AnimState(m_pSelectedObject->Get_Name(), L"Select");
						DWORD SelectedObjectState = m_pSelectedObject->Get_UnitState();

						CUI_Manager::Get_Instance()->SelectUnit(m_pSelectedObject);

						if (SelectedObjectState & UNIT_END)
						{
							break;
						}

						if (SelectedObjectState ^ UNIT_MOVE)
						{
							auto SelectObjectinfo = m_pSelectedObject->Get_UnitInfo();
							//terrain
							m_pTerrain->HighlightTile(m_dwUnitIndex, SelectObjectinfo.iMaxMoveRange, SelectObjectinfo.iAttackRange);
						}
						else if (SelectedObjectState & UNIT_MOVE)
						{
							if (m_bIsMenuUI == false)
							{
								CUI_Manager::Get_Instance()->CreateUnitMenu(m_pSelectedObject);
								m_bIsMenuUI = true;
							}
						}
						//else if (SelectedObjectState ^ UNIT_ATTACK)
						//{
						//	auto SelectObjectinfo = m_pSelectedObject->Get_UnitInfo();
						//	//terrain
						//	m_pTerrain->HighlightTile(m_dwUnitIndex, 0, SelectObjectinfo.iAttackRange);
						//}

						break;
					}
				}

			}
		}
		/*for (auto& it : playerList)
		{
			if (false == it->Get_Turn() || (true == it->Get_Move() && true == it->Get_Attack()))
			{



			}
			else
				EndUnitCount++;
		}*/
		//if (EndUnitCount == playerList.size())
		//{
		//	// 플레이어 턴 끝
		//}

	}
#pragma endregion


#pragma region ENEMY_TURN_SYSTEM
	if (m_eCurTurn == eENEMY_TURN && CScene_Manager::Get_Instance()->Get_CurScene() != CScene_Manager::BATTLE)
	{
		list<CGameObject*>::iterator iterCurEnemy;
		iterCurEnemy = enemyList.begin();
		while ((*iterCurEnemy)->Get_UnitState() & (UNIT_MOVE | UNIT_ATTACK))
		{
			iterCurEnemy++;
			if (iterCurEnemy == enemyList.end())
			{
				Change_Turn();
				return EVENT_NOEVENT;
			}
		}
		// *iterCurEnemy 너무 길어서 m_pSourceObject 로 바꿈
		m_pSourceObject = (*iterCurEnemy);
		DWORD m_pSourceObjectState = (*iterCurEnemy)->Get_UnitState();

		m_pTerrain->DisableHighlightAndClear();
		m_pTerrain->HighlightTile(m_pSourceObject->Get_Locate(), m_pSourceObject->Get_UnitInfo().iCurMoveRange, m_pSourceObject->Get_UnitInfo().iAttackRange);

		if (m_pSourceObject->Get_Target() == nullptr)
		{
			list<int>* AttackableList = m_pTerrain->Get_SelectableList();

			float fMinDist = 1000000;

			//auto& PlayerLocateList = m_pTerrain->Get_PlayerLocateList();

			for each (auto & iterPlayer in playerList)
			{
				auto check = find(AttackableList->begin(), AttackableList->end(), iterPlayer->Get_Locate());
				if (check != AttackableList->end())
				{
					float distX = m_pSourceObject->Get_Info()->vPos.x - iterPlayer->Get_Info()->vPos.x;
					float distY = m_pSourceObject->Get_Info()->vPos.y - iterPlayer->Get_Info()->vPos.y;

					float dist = sqrt(distX * distX + distY * distY);

					if (fMinDist > dist)
					{
						fMinDist = dist;
						m_pSourceObject->Set_Target(iterPlayer);
					}
				}
			}
			if (fMinDist > 100000)
			{
				//m_pSourceObject->Set_Target(nullptr);
				m_pSourceObject->Set_AddUnitState(UNIT_MOVE);
			}
			if (fMinDist < 10000 && fMinDist > 80)
			{
				if (!(m_pSourceObject->Get_UnitState() & UNIT_MOVE)) // move start
				{
					CAStar_Manager::Get_Instance()->StartAStar_Manager(m_pSourceObject->Get_Info()->vPos, m_pSourceObject->Get_Target()->Get_Info()->vPos);
				}
			}
		}

		if (m_pSourceObject->Get_Target() != nullptr)
		{
			list<TILE*>& BestList = CAStar_Manager::Get_Instance()->Get_BestList();
			if (!(m_pSourceObjectState & UNIT_MOVE))
			{
				dynamic_cast<CEnemy*>(m_pSourceObject)->MoveEnemy();
				if (BestList.empty())
				{
					dynamic_cast<CEnemy*>(m_pSourceObject)->MoveEnemyEnd();
					m_pSourceObject->Set_AddUnitState(UNIT_MOVE);
					m_pTerrain->CheckUnitOnTerrain();
				}
			}
		}

		if (((*iterCurEnemy)->Get_UnitState() & UNIT_MOVE) && !((*iterCurEnemy)->Get_UnitState() & UNIT_ATTACK))
		{
			m_pTerrain->HighlightTile(m_pSourceObject->Get_Locate(), 0, m_pSourceObject->Get_UnitInfo().iAttackRange);

			if (m_pSourceObject->Get_Target() != nullptr)
			{
				if (m_pSourceObject->Get_Target()->Get_Locate() != OBJ_DEAD_PLACE)
				{
					// 전투시작
					CScene_Manager::Get_Instance()->Change_Battle_Scene(m_pSourceObject, m_pSourceObject->Get_Target());
				}
			}
			// 전투 끝
			m_pSourceObject->Set_AddUnitState(UNIT_ATTACK);
			m_pSourceObject->Set_AddUnitState(UNIT_END);
			m_pSourceObject->Set_Target(nullptr);
			m_pTerrain->DisableHighlightAndClear();
		}
	}
#pragma endregion


	//if (m_dwCursorState & UNIT_SELECT)
	//{
	if (m_pSelectedObject != nullptr)
	{
		/*CUI_Manager::Get_Instance()->SelectUnit(m_pSelectedObject);*/
	}
	//}
		/*if (CKey_Manager::Get_Instance()->Key_Up(KEY_X))
		{
			CScroll_Manager::Get_Instance()->Move_Scroll_UnitPos();
		}*/
}

void CCombat_Manager::Release_Combat_Manager()
{
}

void CCombat_Manager::Change_Turn()
{
	if (m_eCurTurn == ePLAYER_TURN)
	{
		m_eCurTurn = eENEMY_TURN;
	}
	else if (m_eCurTurn == eENEMY_TURN)
	{
		m_eCurTurn = ePLAYER_TURN;
	}
	TurnSystem_Combat_Manager();
	m_bIsLoading = true;
}

void CCombat_Manager::TurnSystem_Combat_Manager()
{
	if (m_eCurTurn == ePLAYER_TURN)
	{
		for each (CGameObject * pObject in CGameObject_Manager::Get_Instance()->Get_List(PLAYER))
		{
			pObject->Set_ClearUnitState();
			pObject->Set_Target(nullptr);
			pObject->Get_UnitInfo().iCurMoveRange = pObject->Get_UnitInfo().iMaxMoveRange;
		}
		for each (CGameObject * pObject in CGameObject_Manager::Get_Instance()->Get_List(ENEMY))
		{
			pObject->Set_ClearUnitState();
			pObject->Set_Target(nullptr);
			pObject->Get_UnitInfo().iCurMoveRange = pObject->Get_UnitInfo().iMaxMoveRange;
		}

	}
	else if (m_eCurTurn == eENEMY_TURN)
	{

	}
}

void CCombat_Manager::Render_Combat_UI()
{


}

void CCombat_Manager::EnemyToPlayerSetTarget()
{
}

int CCombat_Manager::Get_StageNumber()
{
	if (m_pTerrain)
	{
		return m_pTerrain->Get_StageNumber();
	}
	return 0;
}

void CCombat_Manager::PlayerAttackReady(int iMouseIndex)
{
	auto enemyList = CGameObject_Manager::Get_Instance()->Get_List(ENEMY);

	if (m_AttackableList==nullptr)
		return;

	auto it = find(m_AttackableList->begin(), m_AttackableList->end(), iMouseIndex);
	if (it != m_AttackableList->end())
	{
		if (true == m_pTerrain->Get_TileOnUnit(iMouseIndex))
		{
			for (auto& enemyIndex : enemyList)
			{
				if (enemyIndex->Get_Locate() == iMouseIndex)
				{
					// 전투시작
					m_pTargetObject = enemyIndex;
					m_pSelectedObject->Set_AddUnitState(UNIT_ATTACK | UNIT_END);
					CScene_Manager::Get_Instance()->Change_Battle_Scene(m_pSelectedObject, m_pTargetObject);

					break;
				}
			}
			/////// 공격 가능 범위지만 적이 없을 때

			//m_pTerrain->Set_TileOnUnit(m_pSelectedObject->Get_Locate(), false);
			m_pTerrain->DisableHighlightAndClear();
			m_dwCursorState ^= UNIT_SELECT;
			m_dwUnitIndex = -1;
			m_pSelectedObject->Change_AnimState(m_pSelectedObject->Get_Name(), L"Idle");
			m_dwSelectedMenuIndex = 0;

			//CAStar_Manager::Get_Instance()->StartAStar_Manager(m_pSelectedObject->Get_Info()->vPos, vMouse);
			//pTerrain->Set_TileOnUnit(m_pSelectedObject->Get_Locate(), true);
			m_pSelectedObject = nullptr;

			if (m_bIsMenuUI)
				m_bIsMenuUI = false;
		
		}
	}
	else // 공격 범위가 아닐때
	{
		m_pTerrain->DisableHighlightAndClear();
		m_dwUnitIndex = -1;
		m_dwCursorState ^= UNIT_SELECT;
		m_pSelectedObject->Change_AnimState(m_pSelectedObject->Get_Name(), L"Idle");
		m_pSelectedObject = nullptr;
		m_dwSelectedMenuIndex = 0;
	
		if(m_bIsMenuUI)
			m_bIsMenuUI = false;
	}
}

void CCombat_Manager::Selected_Menu_Attack(CGameObject* pObject)
{
	m_AttackableList = m_pTerrain->Get_AttackAbleList(pObject);
}

void CCombat_Manager::Selected_Menu_Inventory(CGameObject* pObject)
{
	CUI_Manager::Get_Instance()->CreateInventory(pObject);
}

void CCombat_Manager::Selected_Menu_Stay(CGameObject* pObject)
{
	pObject->Set_AddUnitState(UNIT_ATTACK | UNIT_END);
	pObject->Change_AnimState(pObject->Get_Name(), L"Idle");
	
	CloseUI();
}

void CCombat_Manager::CloseUI()
{
	m_pTerrain->DisableHighlightAndClear();
	m_dwUnitIndex = -1;
	m_dwCursorState ^= UNIT_SELECT;
	m_pSelectedObject = nullptr;
	m_dwSelectedMenuIndex = 0;

	if (m_bIsMenuUI)
		m_bIsMenuUI = false;
}
