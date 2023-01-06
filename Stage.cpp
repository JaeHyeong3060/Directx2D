#include "stdafx.h"
#include "Stage.h"
#include "GameObject_Manager.h"
#include "Terrain.h"
#include "Player.h"
#include "Enemy.h"

CStage::CStage()
	:m_pGameObject_Manager(CGameObject_Manager::Get_Instance())
{
}


CStage::~CStage()
{
	Release_Scene();
}

HRESULT CStage::Ready_Scene()
{

	//CEffect_Manager::Get_Instance()->Set_StateEnd(false);

	CSoundMgr::Get_Instance()->StopAll();
	CSoundMgr::Get_Instance()->PlayBGM(L"15 Binding Vow.mp3");

	CGameObject* pObject = new CTerrain; 
	if (FAILED(pObject->Ready_GameObject()))
		return E_FAIL; 
	dynamic_cast<CTerrain*>(pObject)->LoadTileData_Terrain(L"../Data/StageA.dat");
	
	m_pGameObject_Manager->Add_GameObject_Manager(TERRAIN, pObject); 

	CCombat_Manager::Get_Instance()->Ready_Combat_Manager();


	CScroll_Manager::Get_Instance()->Move_Scroll_UnitPos(512);

	CUI_Manager::Get_Instance()->SelectUnit(CGameObject_Manager::Get_Instance()->Get_List(PLAYER).front());


	::ShowWindow(g_hUIStatus, SW_SHOW);


	CEffect_Manager::Get_Instance()->Set_StateEnd(false);


	return S_OK;
}

void CStage::Update_Scene()
{
	m_pGameObject_Manager->Update_GameObject_Manager();
	CUI_Manager::Get_Instance()->Update_UI_Manager_Stage();
	int EVENT = CCombat_Manager::Get_Instance()->Update_Combat_Manager();
	if (EVENT == EVENT_ENEMY_ALL_DEAD)
	{
		CDialog_Manager::Get_Instance()->Set_DialogIndex(ST_SECOND_DIALOG_START, ST_SECOND_DIALOG_END);
		CDialog_Manager::Get_Instance()->Set_NextSceneIndex(CScene_Manager::DIALOG);

		CScene_Manager::Get_Instance()->Change_Scene_Manager(CScene_Manager::DIALOG);
		::ShowWindow(g_hUIStatus, SW_HIDE);
		return;
	}
	CEffect_Manager::Get_Instance()->Update_Effect_Manager();

	if (m_bDialogEndOnceEvent == false)
	{
		if (CDialog_Manager::Get_Instance()->Get_CurKey() == (ST_STAGEA_START + 2))
		{
			m_bDialogEndOnceEvent = true;

			auto EnemyList = CGameObject_Manager::Get_Instance()->Get_List(ENEMY);
			auto PlayerList = CGameObject_Manager::Get_Instance()->Get_List(PLAYER);

			int count = 0;
			for (auto& it : PlayerList)
			{
				it->SetMovePosGameObject(it->Get_Locate() + 3);
			}
			int a = 0;
		}
	}
}

void CStage::Render_Scene()
{
	m_pGameObject_Manager->Render_GameObject_Manager();
	CUI_Manager::Get_Instance()->Render_UI_Manager_Loading();
	CUI_Manager::Get_Instance()->Render_UI_Manager_Stage();
	CEffect_Manager::Get_Instance()->Render_Effect_Manager();

	if (CEffect_Manager::Get_Instance()->Get_StateEnd() == false)
	{
		CEffect_Manager::Get_Instance()->FadeOutEffect();
	}

	CEffect_Manager::Get_Instance()->Render_Effect_Manager();
}

void CStage::Release_Scene()
{
	m_pGameObject_Manager->Destroy_Instance(); 
}
