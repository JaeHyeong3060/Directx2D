#include "stdafx.h"
#include "StageC.h"
#include "Terrain.h"
#include "Player.h"
#include "Enemy.h"

CStageC::CStageC()
	:m_pGameObject_Manager(CGameObject_Manager::Get_Instance())
{
}


CStageC::~CStageC()
{
	Release_Scene();
}

HRESULT CStageC::Ready_Scene()
{
	m_pGameObject_Manager->Release_GameObject_Stage_Object();


	CGameObject* pObject = new CTerrain;
	if (FAILED(pObject->Ready_GameObject()))
		return E_FAIL;
	dynamic_cast<CTerrain*>(pObject)->LoadTileData_Terrain(L"../Data/StageC.dat");

	m_pGameObject_Manager->Add_GameObject_Manager(TERRAIN, pObject);

	CSpawn_Manager::Get_Instance()->Update_Spwan_Manager(CScene_Manager::STAGEC);

	CCombat_Manager::Get_Instance()->Ready_Combat_Manager();


	CScroll_Manager::Get_Instance()->Set_Scroll({ 0.f,0.f,0.f });


	CEffect_Manager::Get_Instance()->Set_StateEnd(false);


	::ShowWindow(g_hUIStatus, SW_SHOW);

	return S_OK;
}

void CStageC::Update_Scene()
{
	m_pGameObject_Manager->Update_GameObject_Manager();
	CUI_Manager::Get_Instance()->Update_UI_Manager_Stage();
	int EVENT = CCombat_Manager::Get_Instance()->Update_Combat_Manager();
	if (EVENT == EVENT_ENEMY_ALL_DEAD)
	{
		CDialog_Manager::Get_Instance()->Set_DialogIndex(ST_THIRD_DIALOG_START, ST_THIRD_DIALOG_END);
		CDialog_Manager::Get_Instance()->Set_NextSceneIndex(CScene_Manager::DIALOG);

		CScene_Manager::Get_Instance()->Change_Scene_Manager(CScene_Manager::DIALOG);
		::ShowWindow(g_hUIStatus, SW_HIDE);
		return;
	}
}

void CStageC::Render_Scene()
{
	m_pGameObject_Manager->Render_GameObject_Manager();
	CUI_Manager::Get_Instance()->Render_UI_Manager_Loading();
	CUI_Manager::Get_Instance()->Render_UI_Manager_Stage();

}

void CStageC::Release_Scene()
{
	m_pGameObject_Manager->Destroy_Instance();
}
