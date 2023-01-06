#include "stdafx.h"
#include "UI_UnitMenu.h"


CUI_UnitMenu::CUI_UnitMenu():
	m_dwSelectedMenu(0)
{
}

CUI_UnitMenu::~CUI_UnitMenu()
{
}

void CUI_UnitMenu::Set_UI_DESC(UI_DESC* pDESC)
{
	m_dwDrawID = pDESC->dwDrawID;
	m_tInfo = pDESC->tUI_Info;
	m_wstrFrameKey = pDESC->wstrFrameKey;
	m_dwCurObjectState = pDESC->dwUnitState;
}

HRESULT CUI_UnitMenu::Ready_GameObject()
{
	m_wstrFrameKey = L"";
	m_eUO = UI_OPTION::UO_MENU;
	return S_OK;
}

int CUI_UnitMenu::Update_GameObject()
{
	if (m_bIsDead)
		return OBJ_DEAD;
	return 0;
}

void CUI_UnitMenu::Late_Update_GameObject()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWND, &pt);
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Instance()->Get_Scroll();
	pt.x -= vScroll.x;
	pt.y -= vScroll.y;

	if (PtInRect(&m_tRectMenu[0], pt))
	{
		if (CKey_Manager::Get_Instance()->Key_Down(KEY_LBUTTON))
		{
			CSoundMgr::Get_Instance()->PlaySound(L"Click0.wav", CSoundMgr::UI);
			CCombat_Manager::Get_Instance()->Selected_Menu_Attack(m_pTarget);
;			m_dwSelectedMenu = 1;
			m_bIsDead = true;
			
		}
	}

	if (PtInRect(&m_tRectMenu[1], pt))
	{
		if (CKey_Manager::Get_Instance()->Key_Down(KEY_LBUTTON))
		{
			CSoundMgr::Get_Instance()->PlaySound(L"Click0.wav", CSoundMgr::UI);
			CCombat_Manager::Get_Instance()->Selected_Menu_Inventory(m_pTarget);
			m_dwSelectedMenu = 2;
			m_bIsDead = true;
		}
	}

	if (PtInRect(&m_tRectMenu[2], pt))
	{
		if (CKey_Manager::Get_Instance()->Key_Down(KEY_LBUTTON))
		{
			CSoundMgr::Get_Instance()->PlaySound(L"Click0.wav", CSoundMgr::UI);
			CCombat_Manager::Get_Instance()->Selected_Menu_Stay(m_pTarget);
			m_dwSelectedMenu = 3;
			m_bIsDead = true;
		}
	}
	CCombat_Manager::Get_Instance()->Set_SeletedMenuIndesx(m_dwSelectedMenu);
}

void CUI_UnitMenu::Render_GameObject()
{
	CUI_UnitMenu::UpdateRect_GameObject();
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_Manager(L"UI", L"MenuUI", 1);

	D3DXMATRIX matScale, matTrans, matWorld;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	matWorld = matScale * matTrans;

	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CUI_UnitMenu::Release_GameObject()
{
}

CGameObject* CUI_UnitMenu::Create(LPVOID* pParam)
{
	CGameObject* pInstance = new CUI_UnitMenu;
	if (0 > pInstance->Ready_GameObject())
	{
		Safe_Delete(pInstance);
		return pInstance;
	}
	if (nullptr != pParam)
		dynamic_cast<CUI_UnitMenu*>(pInstance)->Set_UI_DESC((UI_DESC*)pParam);

	return pInstance;
}

void CUI_UnitMenu::UpdateRect_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_Manager(L"UI", L"MenuUI",1);

	float fScrollX = CScroll_Manager::Get_Instance()->Get_Scroll(CScroll_Manager::eSB_HORZ);
	float fScrollY = CScroll_Manager::Get_Instance()->Get_Scroll(CScroll_Manager::eSB_VERT);

	float fCenterX = 195 / 2;
	float fCenterY = 280 / 2;

	float fOffsetX = m_tInfo.vPos.x - 84.5f;
	float fOffsetXright = m_tInfo.vPos.x + 96.5;
	float fOffsetY = m_tInfo.vPos.y - 102+12;
	float fOffsetYbottom = m_tInfo.vPos.y -29+12;

	for (int i = 0; i < 3; i++)
	{
		float fSpaceY = 73*i;

		m_tRectMenu[i].left = LONG(fOffsetX) - fScrollX;
		m_tRectMenu[i].top = static_cast<LONG>(fOffsetY + fSpaceY) - fScrollY;
		m_tRectMenu[i].right = static_cast<LONG>(fOffsetXright) - fScrollX;
		m_tRectMenu[i].bottom = static_cast<LONG>(fOffsetYbottom + fSpaceY) - fScrollY;
	}

}
