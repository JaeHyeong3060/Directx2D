#include "stdafx.h"
#include "UI_Status.h"
#include "GameObject.h"
void CUI_Status::NotifySelectUnit(CGameObject* pObject)
{
    if (nullptr == pObject )
        return;

    pCurObject = pObject;
    auto& UnitInfo = pObject->Get_UnitInfo();

    TCHAR szBackFilePath[MAX_PATH] = L"";
    swprintf_s(szBackFilePath, L"../Texture/UI/StatusBackground.bmp");



    HBITMAP backgroundbmp = (HBITMAP)LoadImage(NULL, szBackFilePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    SendDlgItemMessage(g_hUIStatus, IDC_BackGround, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)backgroundbmp);


	//printf("%d", UnitInfo.iAtt);

    SetDlgItemInt(g_hUIStatus, IDC_Status, UnitInfo.iCurHp, FALSE);
    SetDlgItemInt(g_hUIStatus, IDC_Status6, UnitInfo.iMaxHp, FALSE);
    SetDlgItemInt(g_hUIStatus, IDC_Status2, UnitInfo.iAtt, FALSE);
    SetDlgItemInt(g_hUIStatus, IDC_Status3, UnitInfo.iDef, FALSE);
    SetDlgItemInt(g_hUIStatus, IDC_Status4, UnitInfo.iMaxMoveRange, FALSE);
    SetDlgItemInt(g_hUIStatus, IDC_Status7, (UnitInfo.iMaxExp - UnitInfo.iExp), FALSE);
    SetDlgItemInt(g_hUIStatus, IDC_Status8, UnitInfo.iLevel, FALSE);
    //SetDlgItemInt(g_hUIStatus, IDC_EDIT5, UnitInfo.byJob, FALSE);
    SetDlgItemTextW(g_hUIStatus, IDC_Status5, pObject->Get_Name().c_str());
    //SetDlgItemInt(g_hUIStatus, IDC_EDIT4, UnitInfo.wstrName, FALSE);
    SetDlgItemTextW(g_hUIStatus, IDC_TestHp, L"HP :");
    SetDlgItemTextW(g_hUIStatus, IDC_TestHp2, L"ATK :");
    SetDlgItemTextW(g_hUIStatus, IDC_TestHp3, L"DEF :");
    SetDlgItemTextW(g_hUIStatus, IDC_TestHp4, L"MOVE :");
    SetDlgItemTextW(g_hUIStatus, IDC_TestHp5, L"NAME :");
    SetDlgItemTextW(g_hUIStatus, IDC_TestHp6, L"/");
    SetDlgItemTextW(g_hUIStatus, IDC_TestHp7, L"NextExp :");
    SetDlgItemTextW(g_hUIStatus, IDC_TestHp8, L"Level :");

    //const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_Manager(L"Eirik", L"Idle", 0);

    TCHAR szFilePath[MAX_PATH] = L"";
    swprintf_s(szFilePath, L"../Texture/Unit/Unit_Face/bmp/Face%d.bmp", UnitInfo.dwDrawId);

    HBITMAP bmp= (HBITMAP)LoadImage(NULL, szFilePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

    SendDlgItemMessage(g_hUIStatus, IDC_UnitFace, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
    //HBITMAP bmp = (HBITMAP)pTexInfo;
    //if (bmp == NULL)
    //{
    //   // MessageBox(NULL, "Error", "ERROR", MB_OK);
    //   // return TRUE;
    //    int a = 0;
    //}
    //SendMessasge(g_hUIStatus, BM_SETIMAGE, 0, (LPARAM)bmp);
    
  

}

void CUI_Status::SelectedUnitMoveEnd(CGameObject* pObject)
{
}
