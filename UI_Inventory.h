#pragma once
class CUI_Inventory
{
public:
	void Set_state(bool state) { m_bIsOpen = state; }
	void Set_Index(wstring wstrKey);
	void Set_Target(CGameObject* pTarget) { m_pTarget = pTarget; }
	bool Get_state() { return m_bIsOpen; }
public:
	void Ready_UI_Inventory();
	void Update_UI_Inventory();
	void Render_UI_Inventory();

public:
	void UpdateRect_GameObject();

	void ADD_InvenItem(CGameObject* pitem);
	
	void SelectedInven(vector<CGameObject*>::iterator iterInven);

	CGameObject* Use_Delete_InvenItem(int iIndex);
private:
	bool m_bIsOpen;

	RECT m_tRectInventory[5]{};
	INFO m_tInfo{};

	int m_iIndex;
	vector<CGameObject*> m_vInventory;
	CGameObject* m_pTarget=nullptr;
};