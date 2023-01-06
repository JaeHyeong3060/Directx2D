#pragma once
class CGameObject; 
class CGameObject_Manager
{
	DECLARE_SINGLETON(CGameObject_Manager)
public:
	
private:
	CGameObject_Manager();
	~CGameObject_Manager();
public:
	CGameObject* Get_Terrain(int StageNum) { 
		auto it = m_listGameObject[TERRAIN].begin();
		for (int i = 0; i < StageNum; i++) 
			it++;
		return (*it);  }
	list<CGameObject*>& Get_List(ID eId) { return m_listGameObject[eId]; }
public:
	HRESULT Add_GameObject_Manager(ID eID, CGameObject* pObject); 
	void Update_GameObject_Manager(); 
	void Render_GameObject_Manager(); 
	void Release_GameObject_Manager(); 
	void Release_GameObject_Stage_Object();

	void Render_GameObject_Manager_UI();
private:
	list<CGameObject*> m_listGameObject[END];
};

