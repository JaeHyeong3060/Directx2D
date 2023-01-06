#include "stdafx.h"
#include "Dialog_Manager.h"

IMPLEMENT_SINGLETON(CDialog_Manager)

HRESULT CDialog_Manager::Ready_Dialog_Manager(const wstring& wstrFilePath)
{
	m_iCurKey = 0;
	m_iMapCount = 0;
	m_iStartIndex = 0;
	m_iEndIndex = 0;
	m_RecentDialogEnd = m_iEndIndex;

	m_iLineCount = 3;
	m_iLineLength = 20;

	m_bIsEnd = false;
	m_eSceneId = CScene_Manager::STAGEA;
	m_bIsInitial = false;
	m_bIsDialogEnd = false;

	wifstream fin;
	fin.open(wstrFilePath);
	
	// 한글 읽기 위해서 필요함 UTF-8 인코딩
	fin.imbue(std::locale("ko_KR.UTF-8"));

	if (fin.fail())
	{
		ERR_MSG(L"뭔가 실패");
		return E_FAIL;
	}
	
	TCHAR szFaceId[MAX_PATH]{};
	TCHAR szwstrName[MAX_PATH]{};
	TCHAR szDialog[500]{};
	while (true)
	{
		fin.getline(szFaceId, MAX_PATH, L'|');
		fin.getline(szwstrName, MAX_PATH, L'|');
		fin.getline(szDialog, MAX_PATH);

		if (fin.eof())
			break;
		DWORD dwCount = _ttoi(szFaceId);

		DIALOG_INFO tmpDialogInfo;
		tmpDialogInfo.dwFaceId = _ttoi(szFaceId);
		tmpDialogInfo.wstrName = szwstrName;
		tmpDialogInfo.wstrDialog = szDialog;
		m_dialogMap.emplace(m_iMapCount, tmpDialogInfo);
		m_iMapCount++;
	}
	fin.close();


	return S_OK;
}

int CDialog_Manager::Update_Dialog_Manager()
{
	if (!m_bIsInitial)
		Awake();
	if (CKey_Manager::Get_Instance()->Key_Up(KEY_Z))
	{
		m_iCurKey++;
		//printf("cur key : %d\n", m_iCurKey);
		if (m_iEndIndex == 0)
		{
			m_iCurKey = 0;
			return OBJ_NOEVENT;
		}
		if (m_iCurKey >= m_iEndIndex)
		{
			m_iStartIndex = 0;
			m_iEndIndex = 0;
			m_iCurKey = 0;
			if(CScene_Manager::Get_Instance()->Get_CurScene() == CScene_Manager::DIALOG)
				CScene_Manager::Get_Instance()->Change_Scene_Manager(m_eSceneId);
			m_bIsDialogEnd = true;
			return EVENT_DIALOG_END;
		}
		for (int i = 0; i < 90; i++)
		{
			m_TempBuf[i] = 0;
		}
		while(true)
		{	
			if (m_dialogMap[m_iCurKey].wstrDialog.length() < m_iLineCount)
			{
				break;
			}
			if (30 < m_iLineLength)
			{
				wcscat_s(m_TempBuf, L"\n");
				m_iLineLength = 0;
			}
			// 문장 한개 추출
			swprintf_s(m_szNextBuf, m_dialogMap[m_iCurKey].wstrDialog.substr(m_iLineCount,1).c_str());
			// 추출한거 붙임
			wcscat_s(m_TempBuf, m_szNextBuf);
			m_iLineCount++;
			m_iLineLength++;
		}
		m_iLineCount = 0;
		m_iLineLength = 0;
	}

}

void CDialog_Manager::Render_Dialog_Manager()
{
	if (m_iCurKey >= m_iEndIndex || m_iStartIndex == 0 || m_iEndIndex == 0)
		return;

	MoveScrollEvent(m_iCurKey);

	D3DXMATRIX matTrans, matScale, matWorld;
	float fCenterX = 0.f;
	float fCenterY = 0.f;

	const TEXINFO* pDialogInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_Manager(L"Dialog", L"DialogImg", 0);

	fCenterX = pDialogInfo->tImageInfo.Width >> 1;
	fCenterY = pDialogInfo->tImageInfo.Height >> 1;
	D3DXMatrixTranslation(&matTrans, fCenterX, fCenterY, 0.f);
	matWorld = matTrans;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pDialogInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	const TEXINFO* pFaceInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_Manager(L"Unit", L"Face", m_dialogMap[m_iCurKey].dwFaceId);

	fCenterX = pFaceInfo->tImageInfo.Width >> 1;
	fCenterY = pFaceInfo->tImageInfo.Height >> 1;
	D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);
	D3DXMatrixTranslation(&matTrans, DIALOG_OffsetX, DIALOG_OffsetY, 0.f);
	matWorld = matScale * matTrans;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pFaceInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMatrixTranslation(&matTrans, 100, 470, 0.f);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
	CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), m_TempBuf, lstrlen(m_TempBuf), nullptr, 0, D3DCOLOR_ARGB(255, 0, 0, 0));


	wstring wsterName = m_dialogMap[m_iCurKey].wstrName;
	D3DXMatrixTranslation(&matTrans, 80, 410, 0.f);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
	CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), wsterName.c_str(), lstrlen(wsterName.c_str()), nullptr, 0, D3DCOLOR_ARGB(255, 0, 0, 0));

}

void CDialog_Manager::Set_DialogIndex(int start, int end)
{
	m_iStartIndex = start; 
	m_iEndIndex = end;

	m_RecentDialogEnd = m_iEndIndex;

	m_iCurKey = m_iStartIndex;
	m_bIsInitial = false;
	m_bIsDialogEnd = false;
}

void CDialog_Manager::Awake()
{
	m_bIsInitial = true;

	if (m_iEndIndex == 0)
		return;
	for (int i = 0; i < 90; i++)
	{
		m_TempBuf[i] = 0;
	}
	while (true)
	{
		if (m_dialogMap[m_iCurKey].wstrDialog.length() < m_iLineCount)
		{
			break;
		}
		if (30 < m_iLineLength)
		{
			wcscat_s(m_TempBuf, L"\n");
			m_iLineLength = 0;
		}
		// 문장 한개 추출
		swprintf_s(m_szNextBuf, m_dialogMap[m_iCurKey].wstrDialog.substr(m_iLineCount, 1).c_str());
		// 추출한거 붙임
		wcscat_s(m_TempBuf, m_szNextBuf);
		m_iLineCount++;
		m_iLineLength++;
	}
	m_iLineCount = 0;
	m_iLineLength = 0;


}

void CDialog_Manager::MoveScrollEvent(int Curkey)
{
	auto playerlist = CGameObject_Manager::Get_Instance()->Get_List(PLAYER);
	auto enemylist = CGameObject_Manager::Get_Instance()->Get_List(ENEMY);
	if (Curkey == ST_STAGEA_START + 1)
		CScroll_Manager::Get_Instance()->Move_Scroll_UnitPos(204);
	if (Curkey == ST_STAGEA_START + 2)
		CScroll_Manager::Get_Instance()->Move_Scroll_UnitPos(204);

	if (Curkey == ST_STAGEB_START)
		CScroll_Manager::Get_Instance()->Move_Scroll_UnitPos(30);
	if(Curkey==ST_STAGEB_START+1)
		CScroll_Manager::Get_Instance()->Move_Scroll_UnitPos(512);
	if (Curkey == ST_STAGEB_START + 2)
		CScroll_Manager::Get_Instance()->Move_Scroll_UnitPos(60);
	if (Curkey == ST_STAGEB_START + 3)
		CScroll_Manager::Get_Instance()->Move_Scroll_UnitPos(512);
}
