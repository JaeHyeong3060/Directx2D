#include "stdafx.h"
#include "MyButton.h"


CMyButton::CMyButton()
{
}

CMyButton::~CMyButton()
{
}

void CMyButton::Set_UI_DESC(UI_DESC* pDESC)
{
	m_dwDrawID		=	pDESC->dwDrawID;
	m_tInfo			=	pDESC->tUI_Info;
	m_wstrFrameKey	=	pDESC->wstrFrameKey;
}

HRESULT CMyButton::Ready_GameObject()
{
	m_wstrFrameKey = L"";
	m_eUO = UI_OPTION::UO_BUTTON;
   return S_OK;
}

int CMyButton::Update_GameObject()
{
    return 0;
}

void CMyButton::Late_Update_GameObject()
{
    POINT pt = {};
    GetCursorPos(&pt);
    ScreenToClient(g_hWND, &pt);
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Instance()->Get_Scroll();
	pt.x -= vScroll.x;
	pt.y -= vScroll.y;

	if (PtInRect(&m_tRect, pt))
	{
		if (CKey_Manager::Get_Instance()->Key_Down(KEY_LBUTTON))
		{
			if (!lstrcmp(m_wstrFrameKey.c_str(), L"TurnEndBtn"))
			{
				CCombat_Manager::Get_Instance()->Change_Turn();
			}
			if (!lstrcmp(m_wstrFrameKey.c_str(), L"Start"))
			{
			//	CScene_Manager::Get_Instance()->Change_Scene_Manager(CScene_Manager::SCENE_STAGE1);
				// Scene�� ����Ǵ� ��Ȳ�̸� �̹� ��ư�� ������ ����. 
				// �̶� �ҽǵ� ������ �ִ� ����� �ǵ帰��?? 
				// �翬 �ȵ���. �̰� �ؾ��������. 
				// ���� �������� �� ������ �ִµ� �������� ���� �� �ϳ� ������ �� �������� �ְ�
				// �� �������� �ְ� ���� ���ڴ�� �ڵ� �ϼ� ��Ű�� ���� 
				// �̷��� �������� �ٷ� �������� �� �հ� �ڵ带 �ۼ�����. 
				return;
			}
			else if (m_wstrFrameKey == L"Edit")
			{
			//	CScene_Manager::Get_Instance()->Change_Scene_Manager(CScene_Manager::SCENE_STAGE2);
				return;
			}
			else if (m_wstrFrameKey == L"Exit")
				DestroyWindow(g_hWND);
		}
		//m_dwDrawID = 1;

	}
	//else
	//	m_dwDrawID = 0;

}

void CMyButton::Render_GameObject()
{
	CMyButton::UpdateRect_GameObject();
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_Manager(L"BUTTON", m_wstrFrameKey);

	D3DXMATRIX matScale, matTrans, matWorld;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y , 0.f);
	matWorld = matScale * matTrans;

	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	{
	
	}

}

void CMyButton::Release_GameObject()
{
}

CGameObject* CMyButton::Create(LPVOID* pParam)
{
	CGameObject* pInstance = new CMyButton;
	if (0 > pInstance->Ready_GameObject())
	{
		Safe_Delete(pInstance);
		return pInstance;
	}
	if (nullptr != pParam)
		dynamic_cast<CMyButton*>(pInstance)->Set_UI_DESC((UI_DESC*)pParam);

	return pInstance;
}

void CMyButton::UpdateRect_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_Manager(L"BUTTON", m_wstrFrameKey);

	float fScrollX = CScroll_Manager::Get_Instance()->Get_Scroll(CScroll_Manager::eSB_HORZ);
	float fScrollY = CScroll_Manager::Get_Instance()->Get_Scroll(CScroll_Manager::eSB_VERT);

	m_tRect.left = LONG(m_tInfo.vPos.x - (pTexInfo->tImageInfo.Width >> 1))- fScrollX;
	m_tRect.top = static_cast<LONG>(m_tInfo.vPos.y- (pTexInfo->tImageInfo.Height >> 1))- fScrollY;
	m_tRect.right = static_cast<LONG>(m_tInfo.vPos.x + (pTexInfo->tImageInfo.Width >> 1))- fScrollX;
	m_tRect.bottom = static_cast<LONG>(m_tInfo.vPos.y + (pTexInfo->tImageInfo.Height >> 1))- fScrollY;

}
