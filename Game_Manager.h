#pragma once
class CGame_Manager
{
	DECLARE_SINGLETON(CGame_Manager)
private:
	CGame_Manager();
public:
	virtual ~CGame_Manager();

	void Add_Gold(int gold) { m_iGold += gold; printf("%d\n", m_iGold); }
	void ADD_InvenItemKey(wstring wstrKey,CGameObject* pitem);
	void Delete_InvenItemKey(wstring wstrKey, CGameObject* pitem);


	vector<CGameObject*> Get_InventoryID(wstring&wstrKey);
	int Get_Gold() { return m_iGold; }
	int Get_KeyInvenSize(wstring wstrKey) { return m_mapItemVector[wstrKey].size(); }
public:
	HRESULT Ready_Game_Manager();
private:
	int m_iGold;

	map<wstring, vector<CGameObject*>> m_mapItemVector;

};