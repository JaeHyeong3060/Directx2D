#include "stdafx.h"
#include "Scene_Manager.h"
#include "Title.h"
#include "DialogScene.h"
#include "Stage.h"
#include "StageB.h"
#include "StageC.h"
#include "ShopScene.h"
#include "BattleScene.h"

IMPLEMENT_SINGLETON(CScene_Manager)
CScene_Manager::CScene_Manager()
	:m_eCurScene(END)
	, m_eNextScene(END)
	, m_pScene(nullptr)
{
}


CScene_Manager::~CScene_Manager()
{
	Release_Scene_Manager(); 
}


HRESULT CScene_Manager::Return_Prev_Scene()
{
	if (m_pPrevScene)
	{
		Safe_Delete(m_pScene);
		m_pScene = m_pPrevScene;
		m_pPrevScene = nullptr;
		m_eCurScene = m_ePrevScene;
		m_ePrevScene = SCENE_ID::END;
	}
	CSoundMgr::Get_Instance()->StopAll();
	CSoundMgr::Get_Instance()->PlayBGM(L"15 Binding Vow.mp3");
	return S_OK;
}

HRESULT CScene_Manager::Change_Battle_Scene(CGameObject* pSource, CGameObject* pDest)
{
	CScene* pBattleScene = CBattleScene::Create(pSource, pDest);
	m_pPrevScene = m_pScene;
	m_pScene = pBattleScene;
	m_ePrevScene = m_eCurScene;
	m_eCurScene = CScene_Manager::BATTLE;
	return S_OK;
}

HRESULT CScene_Manager::Change_Shop_Scene(CGameObject* pObject)
{
	CScene* pShopScene = CShopScene::Create(pObject);
	m_pPrevScene = m_pScene;
	m_pScene = pShopScene;
	m_ePrevScene = m_eCurScene;
	m_eCurScene = CScene_Manager::SHOP;
	return S_OK;
}

HRESULT CScene_Manager::Change_Scene_Manager(SCENE_ID eNextScene)
{
	m_eNextScene = eNextScene; 
	if (m_eNextScene != m_eCurScene)
	{
		if (m_pScene && (m_eCurScene!=STAGEA && m_eCurScene != STAGEB && m_eCurScene != STAGEC))
		{
			Safe_Delete(m_pScene);
		}
		switch (m_eNextScene)
		{
		case CScene_Manager::LOADING:
			break;
		case CScene_Manager::TITLE:
			m_pScene = new CTitle;
			break;
		case CScene_Manager::DIALOG:
			m_pScene = new CDialogScene;
			break;
		case CScene_Manager::STAGEA:
			CDialog_Manager::Get_Instance()->Set_DialogIndex(ST_STAGEA_START, ST_STAGEA_END);
			CDialog_Manager::Get_Instance()->Set_NextSceneIndex(CScene_Manager::DIALOG);
			CSpawn_Manager::Get_Instance()->Update_Spwan_Manager(STAGEA);
			m_pScene = new CStage; 
			break;
		case CScene_Manager::STAGEB:
			CDialog_Manager::Get_Instance()->Set_DialogIndex(ST_STAGEB_START, ST_STAGEB_END);
			CDialog_Manager::Get_Instance()->Set_NextSceneIndex(CScene_Manager::DIALOG);

			m_pScene = new CStageB;
			break;
		case CScene_Manager::STAGEC:
			CDialog_Manager::Get_Instance()->Set_DialogIndex(ST_STAGEC_START, ST_STAGEC_END);
			CDialog_Manager::Get_Instance()->Set_NextSceneIndex(CScene_Manager::DIALOG);
			m_pScene = new CStageC;
			break;
		case CScene_Manager::BATTLE:
			//m_pPrevScene = m_pScene;
			//m_pScene = CBattleScene::Create();
			break;
		case CScene_Manager::END:
			break;
		default:
			break;
		}
		if (FAILED(m_pScene->Ready_Scene()))
		{
			Safe_Delete(m_pScene); 
			return E_FAIL;
		}

		m_eCurScene = m_eNextScene; 
		CScroll_Manager::Get_Instance()->Set_Scroll({ 1.f,1.f,0.f });
	}
	return S_OK;
}

void CScene_Manager::Update_Scene_Manager()
{
	m_pScene->Update_Scene(); 
	CDialog_Manager::Get_Instance()->Update_Dialog_Manager();
	//CUI_Manager::Get_Instance()->Update_UI_Manager();
}

void CScene_Manager::Render_Scene_Manager()
{
	m_pScene->Render_Scene(); 
	CDialog_Manager::Get_Instance()->Render_Dialog_Manager();
}

void CScene_Manager::Release_Scene_Manager()
{
	Safe_Delete(m_pScene); 
}
