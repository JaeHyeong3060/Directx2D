#pragma once
class CUI_Subject;
class CUI_Status;
class CUI_Inventory;

class CUI_Manager
{
	DECLARE_SINGLETON(CUI_Manager)
public:
	HRESULT	Ready_UI_Manager();
	void	Update_UI_Manager();
	void	Update_UI_Manager_Stage();
	void	Render_UI_Manager_Loading();
	void	Render_UI_Manager_Stage();

	void SelectUnit(CGameObject* pObject);

public:
	void CreateUnitMenu(CGameObject* pObject);

	void CreateInventory(CGameObject* pObject);
private:
	float m_fLoadingTime;
	int m_iAlpha;

	CUI_Status* m_pUI_Status;
	CUI_Subject* m_pUI_Subject;

	CUI_Inventory* m_pUI_Inventory;
};