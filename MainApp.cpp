#include "stdafx.h"
#include "MainApp.h"
#include "Scene_Manager.h"
#include "GameObject_Manager.h"
#include "Frame_Manager.h"
CMainApp::CMainApp()
	:m_pGraphic_Device(CGraphic_Device::Get_Instance())
	, m_pScene_Manager(CScene_Manager::Get_Instance())
	, m_pGame_Manager(CGame_Manager::Get_Instance())
{
}


CMainApp::~CMainApp()
{
	Release_MainApp(); 
}

HRESULT CMainApp::Ready_MainApp()
{
	m_pGame_Manager->Ready_Game_Manager();

	vector<wstring> VECTOR_WSTR_UNIT_LIST{ L"Eirik",L"Berserker",L"Thief",L"Paladin",L"SpearMan"};
	vector<wstring> VECTOR_WSTR_ANIM_LIST{ L"Idle",L"Move_Left",L"Move_Front",L"Move_Back",L"Select"};
	vector<int>		ANIM_FRAME_INDEX{ 3,4,4,4,3};

	CTime_Manager::Get_Instance()->Ready_Time_Manager(); 
	if (FAILED(m_pGraphic_Device->Ready_Graphic_Device(CGraphic_Device::MODE_WIN)))
		return E_FAIL; 
	
	int UNIT_LIST_SIZE = VECTOR_WSTR_UNIT_LIST.size();
	int UNIT_ANIM_SIZE = VECTOR_WSTR_ANIM_LIST.size();
	TCHAR szBuffer[MAX_PATH] = L"";
	for (int i = 0; i < UNIT_LIST_SIZE; i++)
	{
		for (int j = 0; j < UNIT_ANIM_SIZE; j++)
		{
			swprintf_s(szBuffer, L"../Texture/Unit/%s/%s%%d.png", VECTOR_WSTR_UNIT_LIST[i].c_str(), VECTOR_WSTR_ANIM_LIST[j].c_str());
			int a = 0;
			if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::TEX_MULTI,
				szBuffer, VECTOR_WSTR_UNIT_LIST[i].c_str(), VECTOR_WSTR_ANIM_LIST[j].c_str(), ANIM_FRAME_INDEX[j])))
				return E_FAIL;
			CAnimClip_Manager::Get_Instance()->Add_Clip(VECTOR_WSTR_UNIT_LIST[i].c_str(), VECTOR_WSTR_ANIM_LIST[j].c_str(), 0, ANIM_FRAME_INDEX[j], AO_LOOP, 0, 0);
		}
		{
			//Move_Right 는 예외처리로 LEFT 이미지를 좌우반전 시켜서 보여줌 (걍 리소스 만들걸 헛짓거리함)
			swprintf_s(szBuffer, L"../Texture/Unit/%s/%s%%d.png", VECTOR_WSTR_UNIT_LIST[i].c_str(),L"Move_Left", 4);
			if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::TEX_MULTI,
				szBuffer, VECTOR_WSTR_UNIT_LIST[i].c_str(), L"Move_Right", 4)))
				return E_FAIL;
			CAnimClip_Manager::Get_Instance()->Add_Clip(VECTOR_WSTR_UNIT_LIST[i].c_str(), L"Move_Right", 0, 4, AO_LOOP, 0, 0);
		}
	}

#pragma region AttackAnim
	if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::TEX_MULTI,
		L"../Texture/Unit/Eirik/Attack/Attack%d.png", L"Eirik", L"Attack", 15)))
		return E_FAIL;
	CAnimClip_Manager::Get_Instance()->Add_Clip(L"Eirik", L"Attack", 0, 15, AO_ONCE_RETURN, 0, 0,12);

	if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::TEX_MULTI,
		L"../Texture/Unit/Berserker/Attack/Attack%d.png", L"Berserker", L"Attack", 21)))
		return E_FAIL;
	CAnimClip_Manager::Get_Instance()->Add_Clip(L"Berserker", L"Attack", 0, 21, AO_ONCE_RETURN, 0, 0,8);

	if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::TEX_MULTI,
		L"../Texture/Unit/Thief/Attack/Attack%d.png", L"Thief", L"Attack", 22)))
		return E_FAIL;
	CAnimClip_Manager::Get_Instance()->Add_Clip(L"Thief", L"Attack", 0, 22, AO_ONCE_RETURN, 0, 0,18);

	if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::TEX_MULTI,
		L"../Texture/Unit/Paladin/Attack/Attack%d.png", L"Paladin", L"Attack", 9)))
		return E_FAIL;
	CAnimClip_Manager::Get_Instance()->Add_Clip(L"Paladin", L"Attack", 0, 9, AO_ONCE_RETURN, 0, 0,5);

	if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::TEX_MULTI,
		L"../Texture/Unit/SpearMan/Attack/Attack%d.png", L"SpearMan", L"Attack", 12)))
		return E_FAIL;
	CAnimClip_Manager::Get_Instance()->Add_Clip(L"SpearMan", L"Attack", 0, 12, AO_ONCE_RETURN, 0, 0,7);

#pragma endregion

	/// Unit FACE
	if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::TEX_MULTI,
		L"../Texture/Unit/Unit_Face/png/Face%d.png", L"Unit", L"Face", 8)))
		return E_FAIL;

	if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::TEX_MULTI,
		L"../Texture/System/Title/Title%d.png", L"Title", L"TitleImg", 5)))
		return E_FAIL;

	if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::TEX_MULTI,
		L"../Texture/System/Dialog/Dialog0.png", L"Dialog", L"DialogImg", 1)))
		return E_FAIL;


	if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::TEX_SINGLE,
		L"../Texture/UI/TurnEndBtn.png", L"BUTTON", L"TurnEndBtn")))
		return E_FAIL;


	if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::TEX_MULTI,
		L"../Texture/Map/Tile/Tile_%d.png", L"Terrain", L"Tile", 10)))
		return E_FAIL;
	
	if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::TEX_MULTI,
		L"../Texture/Map/LocalMap/MapBG0%d.png", L"Terrain", L"LocalMap", STAGEMAX)))
		return E_FAIL;


	if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::TEX_MULTI,
		L"../Texture/UI/TurnUI_%d.png", L"TurnUI", L"TurnUI", 2)))
		return E_FAIL;

	if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::TEX_MULTI,
		L"../Texture/UI/BattleMapUI.png", L"UI", L"BattleMapUI",1)))
		return E_FAIL;

	if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::TEX_MULTI,
		L"../Texture/UI/Digit/digit%d.png", L"UI", L"digit", 10)))
		return E_FAIL;

	if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::TEX_MULTI,
		L"../Texture/UI/MenuUI_%d.png", L"UI", L"MenuUI", 3)))
		return E_FAIL;

	//////////////////////


	if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::TEX_MULTI,
		L"../Texture/System/Effect%d.png", L"Effect", L"Effect", 1)))
		return E_FAIL;

	if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::TEX_MULTI,
		L"../Texture/Item/Item%d.png", L"Item", L"Item", 2)))
		return E_FAIL;


	/// <summary>
	/// /////////////////////////////////////////////////////////////////////////
	/// </summary>
	/// <returns></returns>
	if (FAILED(CDialog_Manager::Get_Instance()->Ready_Dialog_Manager(L"../Data/Dialog.txt")))
		return E_FAIL;
		


	CSpawn_Manager::Get_Instance()->LoadUnitData(L"../Data/UnitData.dat");
	CSpawn_Manager::Get_Instance()->Ready_Spawn_Manager();

	CEffect_Manager::Get_Instance()->Ready_Effect_Manager();
	
	m_pScene_Manager->Change_Scene_Manager(CScene_Manager::TITLE);
	
	//사운드
	CSoundMgr::Get_Instance()->Initialize();
	CSoundMgr::Get_Instance()->PlayBGM(L"01 MainTheme.ogg");


	 //Test용 Stage 시작
	//m_pScene_Manager->Change_Scene_Manager(CScene_Manager::STAGEA);

	
	CUI_Manager::Get_Instance()->Ready_UI_Manager();
	
	return S_OK;

	//TileTexture
	/*if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::TEX_MULTI,
		L"../Texture/Stage/Terrain/Tile/Tile%d.png", L"Terrain", L"Tile", 36)))
		return E_FAIL;*/
		//Player-Attack Texture 
		/*if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::TEX_MULTI,
			L"../Texture/Stage/Player/Attack/AKIHA_AKI01_00%d.png", L"Player", L"Attack", 6)))
			return E_FAIL;*/

}

void CMainApp::Update_MainApp()
{
	CTime_Manager::Get_Instance()->Update_Time_Manager(); 
	CKey_Manager::Get_Instance()->Update_Key_Manager();
	m_pScene_Manager->Update_Scene_Manager(); 
}

void CMainApp::Render_MainApp(CFrame_Manager* pFrame_Manager)
{
	m_pGraphic_Device->Render_Begin(); 
	m_pScene_Manager->Render_Scene_Manager(); 
	//pFrame_Manager->Render_Frame_Manager(); 
	m_pGraphic_Device->Render_End(); 
}

void CMainApp::Release_MainApp()
{
	CSpawn_Manager::Destroy_Instance();
	CDialog_Manager::Destroy_Instance();
	CEffect_Manager::Destroy_Instance();
	CUI_Manager::Destroy_Instance();
	CAStar_Manager::Destroy_Instance();
	CCombat_Manager::Destroy_Instance();
	CTime_Manager::Destroy_Instance(); 
	CScroll_Manager::Destroy_Instance(); 
	CGameObject_Manager::Destroy_Instance(); 
	CKey_Manager::Destroy_Instance();
	CAnimClip_Manager::Destroy_Instance();
	m_pScene_Manager->Destroy_Instance();
	CGame_Manager::Destroy_Instance();
	m_pGraphic_Device->Destroy_Instance(); 
	
}
