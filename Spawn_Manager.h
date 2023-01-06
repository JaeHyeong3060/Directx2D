#pragma once

enum UNIT_TYPE
{
	Eirik,
	Thief,
	Berserker,
	Paladin,
	SpearMan,
	Env,
};

class CSpawn_Manager
{
	DECLARE_SINGLETON(CSpawn_Manager)
public:
	struct SpawnData
	{
		ID eID;
		UNIT_TYPE tUnitType;
		UNIT_DESC tUnitDesc;
		CScene_Manager::SCENE_ID stageID;
	};
public:
	HRESULT Ready_Spawn_Manager();
	HRESULT Update_Spwan_Manager(int StageNumber);
	HRESULT LoadUnitData(const wstring& wstrFilePath);

	void Set_UnitDataIndexByDrawId(UNIT_DESC& tUnidesc,DWORD drawId);
	void Set_UnitDataLotate(UNIT_DESC& tUnidesc, DWORD dwLocate);

	void Set_EnvUnitDESC(UNIT_DESC& tUnitdesc, wstring wstrKey);


public:
	vector<vector<SpawnData>> m_vSpawnList;
	vector<UNITINFO*> m_vUnitInfo;
};

