#pragma once

class CFrame_Manager; 
class CScene_Manager; 
class CMainApp
{
public:
	CMainApp();
	~CMainApp();
public:
	HRESULT Ready_MainApp(); 
	void Update_MainApp(); 
	void Render_MainApp(CFrame_Manager* pFrame_Manager); 
	void Release_MainApp(); 
private:
	CGraphic_Device* m_pGraphic_Device; 
	CScene_Manager* m_pScene_Manager; 
	CGame_Manager* m_pGame_Manager;
};

