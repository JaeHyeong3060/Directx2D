#pragma once

#define UNIT_MOVE		0x0002
#define UNIT_ATTACK		0x0004
#define UNIT_END		0x0008

#define UNIT_MOVING		0x0010
#define UNIT_ATTACKING	0x0020

class CGameObject abstract
{
public:
	explicit CGameObject();
	virtual ~CGameObject();

public:
	void Set_Selected(bool select) { m_bIsSelected = select; }
	void Set_Attack(bool attack) { m_bIsAttack = attack; }
	void Set_Move(bool move) { m_bIsAttack = move; }
	void Set_Moving(bool moving) { m_bIsMoving = moving; }
	void Set_Dead(bool dead) { m_bIsAttack = dead; }
	void Set_Turn(bool turn) { m_bTurn = turn; }
	void Set_Locate(DWORD dwLocate);
	void Set_AddUnitState(DWORD dwState) { m_dwUnitState |= dwState; }
	void Set_ClearUnitState() { m_dwUnitState = 0; }
	void Set_DESC(UNIT_DESC* pDESC);
	void Set_Target(CGameObject* pObject) { m_pTarget = pObject; }
	void Set_eID(ID eID) { m_eId = eID; }
	void Set_Hp(int HpPoint);
	void Set_Name(wstring name) { m_wstrName = name; }


	UNITINFO& Get_UnitInfo() { return m_tUnitInfo; }
	INFO* Get_Info() { return &m_tInfo; }
	wstring Get_Name() { return m_wstrName; }
	bool Get_Selecetd() { return m_bIsSelected; }
	bool Get_Attack() { return m_bIsAttack; }
	bool Get_Move() { return m_bIsMove; }
	bool Get_Moving() { return m_bIsMoving; }
	bool Get_Turn() { return m_bTurn; }
	DWORD Get_UnitState() { return m_dwUnitState; }
	DWORD Get_Locate() { return m_dwLocate; }
	CGameObject* Get_Target() { return m_pTarget; }
	ID Get_ID() { return m_eId; }
	
public:
	void FrameMove(float fSpeed = 1.f);
	void Change_AnimState(wstring objectKey,wstring stateKey);
	int AttackedObject(CGameObject* pObject);

	void SetMovePosGameObject(int movePosIndex);
	void PosFrameMove();

	int Add_Exp(int iExp);

	const RECT* CGameObject::Get_Rect() const
	{
		return &m_tRect;
	}

	
public:
	virtual HRESULT Ready_GameObject()PURE; 
	virtual int Update_GameObject()PURE; 
	virtual void Late_Update_GameObject()PURE; 
	virtual void Render_GameObject()PURE; 
	virtual void Release_GameObject() PURE; 

protected:
	ID m_eId;

	list<TILE*> m_PosMoveList;

	UNITINFO m_tUnitInfo;
	bool m_bIsSelected;
	bool m_bIsAttack;
	bool m_bIsMove;
	bool m_bIsMoving;
	bool m_bTurn;
	bool m_bIsDead;
	float m_fSpeed;
	RECT m_tRect;
	INFO m_tInfo;
	ANIMCLIP* m_pNextAnimClip;
	ANIMCLIP* m_pCurAnimClip;
	DWORD	m_dwLocate;
	DWORD m_dwUnitState;
	wstring m_wstrName;

	CGameObject* m_pTarget;
};

