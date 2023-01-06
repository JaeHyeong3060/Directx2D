#include "stdafx.h"
#include "GameObject_Manager.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CGameObject_Manager)
CGameObject_Manager::CGameObject_Manager()
{
}


CGameObject_Manager::~CGameObject_Manager()
{
	Release_GameObject_Manager(); 
}

HRESULT CGameObject_Manager::Add_GameObject_Manager(ID eID, CGameObject * pObject)
{
	if (eID >= END || nullptr == pObject)
		return E_FAIL; 
	m_listGameObject[eID].emplace_back(pObject); 
	return S_OK;
}

void CGameObject_Manager::Update_GameObject_Manager()
{
	for (int i = 0 ; i < END ; ++i)
	{
		for (auto& iter = m_listGameObject[i].begin(); iter != m_listGameObject[i].end();)
		{
			int iEvent = (*iter)->Update_GameObject(); 
			if (OBJ_DEAD == iEvent)
			{
				Safe_Delete(*iter);
				iter = m_listGameObject[i].erase(iter);
			}
			else
				++iter; 
		}
		for (auto& pGameObject : m_listGameObject[i])
		{
			pGameObject->Late_Update_GameObject(); 
		}
	}
}

void CGameObject_Manager::Render_GameObject_Manager()
{
	for (int i = 0 ; i < END ; ++i)
	{
		if (i == UI)
			continue;
		for (auto& pGameObject : m_listGameObject[i])
		{
			pGameObject->Render_GameObject(); 
		}
	}
}

void CGameObject_Manager::Release_GameObject_Manager()
{
	for (int i = 0 ; i < END ; ++i)
	{
		for (auto& pGameObject : m_listGameObject[i])
		{
			Safe_Delete(pGameObject); 
		}
		m_listGameObject[i].clear(); 
	}
}

void CGameObject_Manager::Render_GameObject_Manager_UI()
{
	for (auto& pGameObject : m_listGameObject[UI])
	{
		pGameObject->Render_GameObject();
	}
}


void CGameObject_Manager::Release_GameObject_Stage_Object()
{
	for (auto& pGameObject : m_listGameObject[PLAYER])
	{
			Safe_Delete(pGameObject);
	}
	m_listGameObject[PLAYER].clear();
	for (auto& pGameObject : m_listGameObject[TERRAIN])
	{
		Safe_Delete(pGameObject);
	}
	m_listGameObject[TERRAIN].clear();
	for (auto& pGameObject : m_listGameObject[ENEMY])
	{
		Safe_Delete(pGameObject);
	}
	m_listGameObject[ENEMY].clear();
	for (auto& pGameObject : m_listGameObject[ENV])
	{
		Safe_Delete(pGameObject);
	}
	m_listGameObject[ENV].clear();
}