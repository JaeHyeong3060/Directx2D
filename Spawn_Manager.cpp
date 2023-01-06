#include "stdafx.h"
#include "Spawn_Manager.h"
#include "Player.h"
#include "Enemy.h"
#include "EnvObject.h"

IMPLEMENT_SINGLETON(CSpawn_Manager)

HRESULT CSpawn_Manager::Ready_Spawn_Manager()
{
	m_vSpawnList.resize(100);

	UNIT_DESC tUnitDesc;
	Set_UnitDataIndexByDrawId(tUnitDesc, 0);
	Set_UnitDataLotate(tUnitDesc, 204);

	m_vSpawnList[CScene_Manager::SCENE_ID::STAGEA].push_back(
		SpawnData{ ID::PLAYER, UNIT_TYPE::Eirik, tUnitDesc,	CScene_Manager::SCENE_ID::STAGEA });

	Set_UnitDataIndexByDrawId(tUnitDesc, 2);
	Set_UnitDataLotate(tUnitDesc, 238);

	m_vSpawnList[CScene_Manager::SCENE_ID::STAGEA].push_back(
		SpawnData{ ID::PLAYER, UNIT_TYPE::Thief, tUnitDesc,	CScene_Manager::SCENE_ID::STAGEA });

	Set_UnitDataIndexByDrawId(tUnitDesc, 3);
	Set_UnitDataLotate(tUnitDesc, 273); //513

	m_vSpawnList[CScene_Manager::SCENE_ID::STAGEA].push_back(
		SpawnData{ ID::PLAYER, UNIT_TYPE::Paladin, tUnitDesc,	CScene_Manager::SCENE_ID::STAGEA });



#pragma region STAGE_A_ENEMY
	Set_UnitDataIndexByDrawId(tUnitDesc, 1);
	Set_UnitDataLotate(tUnitDesc, 182);

	m_vSpawnList[CScene_Manager::SCENE_ID::STAGEA].push_back(
		SpawnData{ ID::ENEMY, UNIT_TYPE::Berserker, tUnitDesc,	CScene_Manager::SCENE_ID::STAGEA });


	Set_UnitDataIndexByDrawId(tUnitDesc, 4);
	Set_UnitDataLotate(tUnitDesc, 184);

	m_vSpawnList[CScene_Manager::SCENE_ID::STAGEA].push_back(
		SpawnData{ ID::ENEMY, UNIT_TYPE::SpearMan, tUnitDesc,	CScene_Manager::SCENE_ID::STAGEA });


	Set_UnitDataIndexByDrawId(tUnitDesc, 4);
	Set_UnitDataLotate(tUnitDesc, 177);

	m_vSpawnList[CScene_Manager::SCENE_ID::STAGEA].push_back(
		SpawnData{ ID::ENEMY, UNIT_TYPE::SpearMan, tUnitDesc,	CScene_Manager::SCENE_ID::STAGEA });


#pragma endregion

#pragma region STAGE_B_ENEMY
	Set_UnitDataIndexByDrawId(tUnitDesc, 4);
	Set_UnitDataLotate(tUnitDesc, 4);

	m_vSpawnList[CScene_Manager::SCENE_ID::STAGEB].push_back(
		SpawnData{ ID::ENEMY, UNIT_TYPE::SpearMan, tUnitDesc,	CScene_Manager::SCENE_ID::STAGEB });

	Set_UnitDataIndexByDrawId(tUnitDesc, 4);
	Set_UnitDataLotate(tUnitDesc, 39);

	m_vSpawnList[CScene_Manager::SCENE_ID::STAGEB].push_back(
		SpawnData{ ID::ENEMY, UNIT_TYPE::SpearMan, tUnitDesc,	CScene_Manager::SCENE_ID::STAGEB });

	Set_UnitDataIndexByDrawId(tUnitDesc, 4);
	Set_UnitDataLotate(tUnitDesc, 6);

	m_vSpawnList[CScene_Manager::SCENE_ID::STAGEB].push_back(
		SpawnData{ ID::ENEMY, UNIT_TYPE::SpearMan, tUnitDesc,	CScene_Manager::SCENE_ID::STAGEB });

	Set_UnitDataIndexByDrawId(tUnitDesc, 1);
	Set_UnitDataLotate(tUnitDesc, 7+34+34);

	m_vSpawnList[CScene_Manager::SCENE_ID::STAGEB].push_back(
		SpawnData{ ID::ENEMY, UNIT_TYPE::SpearMan, tUnitDesc,	CScene_Manager::SCENE_ID::STAGEB });

	Set_UnitDataIndexByDrawId(tUnitDesc, 4);
	Set_UnitDataLotate(tUnitDesc, 9 + 34 + 34);

	m_vSpawnList[CScene_Manager::SCENE_ID::STAGEB].push_back(
		SpawnData{ ID::ENEMY, UNIT_TYPE::SpearMan, tUnitDesc,	CScene_Manager::SCENE_ID::STAGEB });

	Set_UnitDataIndexByDrawId(tUnitDesc, 3);
	Set_UnitDataLotate(tUnitDesc, 11 + 34 + 34);

	m_vSpawnList[CScene_Manager::SCENE_ID::STAGEB].push_back(
		SpawnData{ ID::ENEMY, UNIT_TYPE::SpearMan, tUnitDesc,	CScene_Manager::SCENE_ID::STAGEB });

	Set_UnitDataIndexByDrawId(tUnitDesc, 3);
	Set_UnitDataLotate(tUnitDesc, 15 + 34 + 34);

	m_vSpawnList[CScene_Manager::SCENE_ID::STAGEB].push_back(
		SpawnData{ ID::ENEMY, UNIT_TYPE::SpearMan, tUnitDesc,	CScene_Manager::SCENE_ID::STAGEB });

	Set_UnitDataIndexByDrawId(tUnitDesc, 3);
	Set_UnitDataLotate(tUnitDesc, 15 + 34 + 34 + 34 + 34);

	m_vSpawnList[CScene_Manager::SCENE_ID::STAGEB].push_back(
		SpawnData{ ID::ENEMY, UNIT_TYPE::SpearMan, tUnitDesc,	CScene_Manager::SCENE_ID::STAGEB });

	Set_UnitDataIndexByDrawId(tUnitDesc, 3);
	Set_UnitDataLotate(tUnitDesc, 15 + 34 + 34 + 34 + 34 +34);

	m_vSpawnList[CScene_Manager::SCENE_ID::STAGEB].push_back(
		SpawnData{ ID::ENEMY, UNIT_TYPE::SpearMan, tUnitDesc,	CScene_Manager::SCENE_ID::STAGEB });


	
	
	
	
	Set_UnitDataIndexByDrawId(tUnitDesc, 3);
	Set_UnitDataLotate(tUnitDesc, 512);

	m_vSpawnList[CScene_Manager::SCENE_ID::STAGEB].push_back(
		SpawnData{ ID::PLAYER, UNIT_TYPE::Paladin, tUnitDesc,	CScene_Manager::SCENE_ID::STAGEB });

#pragma endregion


	Set_UnitDataIndexByDrawId(tUnitDesc, 4);
	Set_UnitDataLotate(tUnitDesc, 175);

	m_vSpawnList[CScene_Manager::SCENE_ID::STAGEC].push_back(
		SpawnData{ ID::ENEMY, UNIT_TYPE::SpearMan, tUnitDesc,	CScene_Manager::SCENE_ID::STAGEC });

	Set_UnitDataIndexByDrawId(tUnitDesc, 3);
	Set_UnitDataLotate(tUnitDesc, 40); //513

	m_vSpawnList[CScene_Manager::SCENE_ID::STAGEC].push_back(
		SpawnData{ ID::PLAYER, UNIT_TYPE::Paladin, tUnitDesc,	CScene_Manager::SCENE_ID::STAGEC });


	/// Env Object Create

	Set_EnvUnitDESC(tUnitDesc, L"VilliageEnv0");
	Set_UnitDataLotate(tUnitDesc, 73);

	m_vSpawnList[CScene_Manager::SCENE_ID::STAGEA].push_back(
		SpawnData{ ID::ENV, UNIT_TYPE::Env, tUnitDesc,	CScene_Manager::SCENE_ID::STAGEA });

	Set_EnvUnitDESC(tUnitDesc, L"VilliageEnv0");
	Set_UnitDataLotate(tUnitDesc, 76);

	m_vSpawnList[CScene_Manager::SCENE_ID::STAGEA].push_back(
		SpawnData{ ID::ENV, UNIT_TYPE::Env, tUnitDesc,	CScene_Manager::SCENE_ID::STAGEA });

	Set_EnvUnitDESC(tUnitDesc, L"VilliageEnv0");
	Set_UnitDataLotate(tUnitDesc, 410);

	m_vSpawnList[CScene_Manager::SCENE_ID::STAGEA].push_back(
		SpawnData{ ID::ENV, UNIT_TYPE::Env, tUnitDesc,	CScene_Manager::SCENE_ID::STAGEA });

	Set_EnvUnitDESC(tUnitDesc, L"Shop0");
	Set_UnitDataLotate(tUnitDesc, 278);

	m_vSpawnList[CScene_Manager::SCENE_ID::STAGEA].push_back(
		SpawnData{ ID::ENV, UNIT_TYPE::Env, tUnitDesc,	CScene_Manager::SCENE_ID::STAGEA });

	Set_EnvUnitDESC(tUnitDesc, L"Shop0");
	Set_UnitDataLotate(tUnitDesc, 381);

	m_vSpawnList[CScene_Manager::SCENE_ID::STAGEA].push_back(
		SpawnData{ ID::ENV, UNIT_TYPE::Env, tUnitDesc,	CScene_Manager::SCENE_ID::STAGEA });

	Set_EnvUnitDESC(tUnitDesc, L"Shop0");
	Set_UnitDataLotate(tUnitDesc, 215);

	m_vSpawnList[CScene_Manager::SCENE_ID::STAGEA].push_back(
		SpawnData{ ID::ENV, UNIT_TYPE::Env, tUnitDesc,	CScene_Manager::SCENE_ID::STAGEA });


	Set_EnvUnitDESC(tUnitDesc, L"Exit0");
	Set_UnitDataLotate(tUnitDesc, 513+12);

	m_vSpawnList[CScene_Manager::SCENE_ID::STAGEB].push_back(
		SpawnData{ ID::ENV, UNIT_TYPE::Env, tUnitDesc,	CScene_Manager::SCENE_ID::STAGEB });
	Set_EnvUnitDESC(tUnitDesc, L"Exit0");
	Set_UnitDataLotate(tUnitDesc, 513 +12 + 34);

	m_vSpawnList[CScene_Manager::SCENE_ID::STAGEB].push_back(
		SpawnData{ ID::ENV, UNIT_TYPE::Env, tUnitDesc,	CScene_Manager::SCENE_ID::STAGEB });
	Set_EnvUnitDESC(tUnitDesc, L"Exit0");
	Set_UnitDataLotate(tUnitDesc, 513 +12 + 34+34);

	m_vSpawnList[CScene_Manager::SCENE_ID::STAGEB].push_back(
		SpawnData{ ID::ENV, UNIT_TYPE::Env, tUnitDesc,	CScene_Manager::SCENE_ID::STAGEB });

	

	//UNIT_DESC* player_desc;

	return S_OK;
}

HRESULT CSpawn_Manager::Update_Spwan_Manager(int StageNumber)
{
	for each (auto& it in m_vSpawnList[StageNumber])
	{
		UNIT_DESC* pUnitDesc = const_cast<UNIT_DESC*>(&it.tUnitDesc);
		CGameObject* pObject = nullptr;
		ID CurID = it.eID;
		switch (CurID)
		{
		case ENV:
			pObject = CEnvObject::Create((LPVOID*)pUnitDesc);
			pObject->Set_eID(ENV);
			break;
		case PLAYER:
			pObject = CPlayer::Create((LPVOID*)pUnitDesc);
			pObject->Set_eID(PLAYER);
			break;
		case ENEMY:
			pObject = CEnemy::Create((LPVOID*)pUnitDesc);
			pObject->Set_eID(ENEMY);
			break;
		default:
			break;
		}
		
		if (FAILED(pObject->Ready_GameObject()))
			return E_FAIL;
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(it.eID, pObject);


		/*	if(it.eID==PLAYER)
				pObject = CPlayer::Create((LPVOID*)pUnitDesc);
			else if(it.eID==ENEMY)
				pObject = CEnemy::Create((LPVOID*)pUnitDesc);
			else if(it.eID==ENV)
				pObject = CEnvObject::Create((LPVOID*)pUnitDesc);*/
	}

	
}

HRESULT CSpawn_Manager::LoadUnitData(const wstring& wstrFilePath)
{
	HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwByte = 0;

	DWORD dwStringCount = 0;
	TCHAR* szBuf;
	UNITINFO* pUnitInfo;
	while (true)
	{
		ReadFile(hFile, &dwStringCount, sizeof(DWORD), &dwByte, nullptr);
		if (0 == dwByte)
			break;
		szBuf = new TCHAR[dwStringCount];
		pUnitInfo = new UNITINFO;
		ReadFile(hFile, szBuf, dwStringCount, &dwByte, nullptr);
		pUnitInfo->wstrName = szBuf;
		Safe_Delete_Array(szBuf);
		ReadFile(hFile, &pUnitInfo->iAtt, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &pUnitInfo->iDef, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &pUnitInfo->iMaxHp, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &pUnitInfo->iCurHp, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &pUnitInfo->iMaxMoveRange, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &pUnitInfo->iCurMoveRange, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &pUnitInfo->iAttackRange, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &pUnitInfo->dwDrawId, sizeof(DWORD), &dwByte, nullptr);

		ReadFile(hFile, &pUnitInfo->byJob, sizeof(BYTE), &dwByte, nullptr);
		ReadFile(hFile, &pUnitInfo->byItem, sizeof(BYTE), &dwByte, nullptr);

		pUnitInfo->iExp = 0;
		pUnitInfo->iMaxExp = 20;
		pUnitInfo->iLevel = 1;

		m_vUnitInfo.emplace_back(pUnitInfo);
		//m_mapUnitInfo.emplace(pUnitInfo->wstrName, pUnitInfo);
		//m_ListBox.AddString(pUnitInfo->wstrName);
	}

	CloseHandle(hFile);
	int a = 0;
	return S_OK;
}

void CSpawn_Manager::Set_UnitDataIndexByDrawId(UNIT_DESC& tUnitdesc, DWORD drawId)
{
	auto UnitDataIndex = find_if(m_vUnitInfo.begin(), m_vUnitInfo.end(), [&](UNITINFO* unitInfo) {return unitInfo->dwDrawId == drawId; });
	tUnitdesc.tUnitInfo = *(*UnitDataIndex);
	tUnitdesc.wstr_name = (*(*UnitDataIndex)).wstrName;
}

void CSpawn_Manager::Set_UnitDataLotate(UNIT_DESC& tUnidesc, DWORD dwLocate)
{
	INFO tInfo;

	float PosX = (dwLocate % TILEX) * TILECX;
	float PosY = (dwLocate / TILEX) * TILECY;

	tInfo.vPos = { PosX, PosY, 0.f };
	tInfo.vDir = { 1.f, 1.f, 0.f };
	tInfo.vSize = { 3.0f,3.0f, 0.f };

	tUnidesc.dwLocate = dwLocate;
	tUnidesc.tInfo = tInfo;
}

void CSpawn_Manager::Set_EnvUnitDESC(UNIT_DESC& tUnitdesc,wstring wstrKey)
{
	tUnitdesc.wstr_name = wstrKey;
}


