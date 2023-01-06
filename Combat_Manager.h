#pragma once

// 액션
#define UNIT_SELECT	0x0001
#define UNIT_MOVE	0x0002
#define UNIT_ATTACK 0x0004

// 상황
#define PLAYER_TURN	0x0001
#define EMENY_TURN	0x0002


enum SYSTEM_TURN{ePLAYER_TURN,eENEMY_TURN, eLOADING_TURN,eETC};

class CTerrain;
class CGameObject;
class CCombat_Manager
{
public:
	DECLARE_SINGLETON(CCombat_Manager)
public:
	void Ready_Combat_Manager();
	int Update_Combat_Manager();
	void Release_Combat_Manager();

	void Change_Turn();
	void TurnSystem_Combat_Manager();
	void Render_Combat_UI();
public:
	void EnemyToPlayerSetTarget();
public:
	void Set_SeletedMenuIndesx(DWORD dwMenuIndex) { m_dwSelectedMenuIndex = dwMenuIndex; }
	void Set_CursorState(DWORD dwState) { m_dwCursorState = dwState; }
	void Set_GameState(DWORD dwState) { m_dwGameState = dwState; }
	//void Set_PlayerTurn() { eCurTurn = ePLAYER_TURN; }
	//void Set_EnemyTurn() { eCurTurn = eENEMY_TURN; }
	void Set_Loading(bool load) { m_bIsLoading = load; }
	bool Get_Loading() { return m_bIsLoading; }
	SYSTEM_TURN Get_CurTurn() { return m_eCurTurn; }
	CGameObject* Get_SelectedObject() { return m_pSelectedObject; }
	DWORD Get_GameState() { return m_dwGameState; }
	DWORD Get_CursorState() { return m_dwCursorState; }

	int Get_StageNumber();
public:
	void PlayerAttackReady(int iMouseIndex);
	void Selected_Menu_Attack(CGameObject* pObject);
	void Selected_Menu_Inventory(CGameObject* pObject);
	void Selected_Menu_Stay(CGameObject* pObject);

	void CloseUI();
private:
	CGameObject* m_pSelectedObject;
	CGameObject* m_pTargetObject;
	CGameObject* m_pSourceObject;
	CTerrain* m_pTerrain;
	DWORD m_dwUnitIndex;
	DWORD m_dwGameState;
	DWORD m_dwCursorState;
	DWORD m_dwStageNumber;

	SYSTEM_TURN m_eCurTurn;

	bool bIsEnemyTurnEnd;


	bool m_bIsMenuUI;
	bool m_bIsLoading;

	bool m_IsEnemyAllDead;
	bool m_IsPlayerAllDead;

	DWORD m_dwSelectedMenuIndex;

	list<int>* m_AttackableList;
};
