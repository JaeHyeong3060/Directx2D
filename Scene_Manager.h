#pragma once
class CScene; 
class CScene_Manager
{
	DECLARE_SINGLETON(CScene_Manager)
public:
	enum SCENE_ID { TITLE, DIALOG,LOADING, STAGEA, STAGEB,STAGEC,BATTLE, SHOP, END};
private:
	CScene_Manager();
	~CScene_Manager();
public:
	SCENE_ID Get_CurScene() { return m_eCurScene; }
public:
	HRESULT Return_Prev_Scene();
	HRESULT Change_Scene_Manager(SCENE_ID eNextScene); 
	void	Update_Scene_Manager();
	void	Render_Scene_Manager();
	void	Release_Scene_Manager();
public:
	HRESULT Change_Battle_Scene(CGameObject* pSource, CGameObject* pDest);
	HRESULT Change_Shop_Scene(CGameObject* pObject);
private:
	SCENE_ID m_eCurScene; 
	SCENE_ID m_eNextScene; 
	SCENE_ID m_ePrevScene;
	CScene* m_pScene; 
	CScene* m_pPrevScene;

};

