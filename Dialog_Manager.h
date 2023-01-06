#pragma once

class CDialog_Manager
{
	DECLARE_SINGLETON(CDialog_Manager)
public:
	DIALOG_INFO* Get_DialogInfo(int key) { return &m_dialogMap[key]; }
	int Get_CurKey() { return m_iCurKey; }
	int Get_RecentDialogEnd() { return m_RecentDialogEnd; }
	bool Get_IsDialogEnd() { return m_bIsDialogEnd; }

	void Add_KeyNumber(int number) { m_iCurKey += number; }
	void Set_DialogIndex(int start, int end);
	void Set_NextSceneIndex(CScene_Manager::SCENE_ID eId) { m_eSceneId = eId; };

	void MoveScrollEvent(int Curkey);


	CScene_Manager::SCENE_ID Get_NextSceneId() { return m_eSceneId; }

public:
	HRESULT Ready_Dialog_Manager(const wstring& wstrFilePath);
	int Update_Dialog_Manager();
	void Render_Dialog_Manager();
	
	void Awake();

private:
	TCHAR m_szNextBuf[320]{};
	TCHAR m_szPrevBuf[32]{};
	TCHAR m_TempBuf[320]{};
private:
	int m_iLineCount;
	int m_iLineLength;
	bool m_bIsEnd;

	map<int, DIALOG_INFO> m_dialogMap;
	int m_iMapCount;
	int m_iCurKey;

	CScene_Manager::SCENE_ID m_eSceneId;
	int m_iStartIndex;
	int m_iEndIndex;
	int m_RecentDialogEnd;

	bool m_bIsInitial;

	bool m_bIsDialogEnd;
};

