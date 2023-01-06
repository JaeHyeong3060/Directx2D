#pragma once
#include "GameObject.h"
class CTerrain final : public CGameObject
{
public:
	CTerrain();
public:
	virtual ~CTerrain();
public:
	// 니들은 const 쓰셈. 난 아니야.. 난 자유롭게 살고싶어. 
	const vector<TILE*>& Get_VecTile() const 
	{ return m_vecTile; }
	vector<list<TILE*>>& Get_Graph() { return m_vecGraph; }
	int Get_TileIndex(const D3DXVECTOR3& vPos);
	int Get_StageNumber() { return m_dwStageNumber; }
public:
	void Set_TileOnUnit(int iIndex, bool bOnUnit) { m_DontMoveIndexList.push_back(iIndex); m_vecTile[iIndex]->bOnUnit = bOnUnit; };
	bool Get_TileOnUnit(int iIndex) { return m_vecTile[iIndex]->bOnUnit; }
	list<int>& Get_PlayerLocateList(){ return m_PlayerLocateList; }
	list<int>& Get_EnemyLocateList() { return m_EnemyLocateList; }
public:
	HRESULT Create_Graph_Terrain();
	HRESULT LoadTileData_Terrain(const wstring& wstrFilePath); 
	list<int>* Get_SelectableList() { return &m_SelectableIndexList; }
	list<int>* Get_AttackAbleList(CGameObject* pUnitInfo);
	void HighlightTile(int iIndex,int iMoveRange, int iAttackRange);
	void DisableHighlightAndClear();
	void CheckUnitOnTerrain();
	void Update_Graph_Terrain();
private:
	void ClearTileState();
	void CalculateRange(int CurIndex, int MaxDist, int CurDist);
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
private:
	vector<TILE*> m_vecTile; 
	list<int> m_SelectableIndexList;
	list<int> m_AttackAbleIndexList;

	list<int> m_DontMoveIndexList;

	vector<list<TILE*>> m_vecGraph; 

	list<int> m_PlayerLocateList = {};
	list<int> m_EnemyLocateList = {};

	int m_MinDist[TILEX * TILEY] = {};

	DWORD m_dwStageNumber;

	bool m_bIsDrawId;
};

