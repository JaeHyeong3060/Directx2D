#include "stdafx.h"
#include "StageB.h"
#include "Terrain.h"
#include "Player.h"
#include "Enemy.h"

CStageB::CStageB()
	:m_pGameObject_Manager(CGameObject_Manager::Get_Instance())
{
}


CStageB::~CStageB()
{
	Release_Scene();
}

HRESULT CStageB::Ready_Scene()
{
	m_pGameObject_Manager->Release_GameObject_Stage_Object();


	CGameObject* pObject = new CTerrain;
	if (FAILED(pObject->Ready_GameObject()))
		return E_FAIL;
	dynamic_cast<CTerrain*>(pObject)->LoadTileData_Terrain(L"../Data/StageB.dat");

	m_pGameObject_Manager->Add_GameObject_Manager(TERRAIN, pObject);

	CSpawn_Manager::Get_Instance()->Update_Spwan_Manager(CScene_Manager::STAGEB);

	CCombat_Manager::Get_Instance()->Ready_Combat_Manager();


	CScroll_Manager::Get_Instance()->Set_Scroll({ 0.f,0.f,0.f });


	CEffect_Manager::Get_Instance()->Set_StateEnd(false);


	::ShowWindow(g_hUIStatus, SW_SHOW);	

	return S_OK;
}

void CStageB::Update_Scene()
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

	if (m_bDialogEndOnceEvent == false)
	{
		if (CDialog_Manager::Get_Instance()->Get_CurKey()==(ST_STAGEB_END-2))
		{
			m_bDialogEndOnceEvent = true;

			auto EnemyList = CGameObject_Manager::Get_Instance()->Get_List(ENEMY);

			int count = 0;
			for (auto& it : EnemyList)
			{
				/*auto CurPos = it->Get_Info()->vPos;
				D3DVECTOR vPos = { TILECX*1.f,0.f,0.f };
				auto NextPos = CurPos - vPos;

				it->Get_Locate();*/
				if (count < 7)
					it->SetMovePosGameObject(it->Get_Locate() + 3 * TILEX);
				else
					it->SetMovePosGameObject(it->Get_Locate() - 5);
				count++;
			}
		}
	}
}

void CStageB::Render_Scene()
{
	m_pGameObject_Manager->Render_GameObject_Manager();
	CUI_Manager::Get_Instance()->Render_UI_Manager_Loading();
	CUI_Manager::Get_Instance()->Render_UI_Manager_Stage();

}

void CStageB::Release_Scene()
{
	m_pGameObject_Manager->Destroy_Instance();
}
