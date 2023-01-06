#include "stdafx.h"
#include "UI_Inventory.h"
#include "Item.h"

#include "Player.h"

void CUI_Inventory::Set_Index(wstring wstrKey)
{
	m_vInventory = CGame_Manager::Get_Instance()->Get_InventoryID(wstrKey);
}

void CUI_Inventory::Ready_UI_Inventory()
{
	m_tInfo.vPos = { 200.f,200.f,0.f };
	m_tInfo.vSize={ 1.f,1.f,0.f };
	m_bIsOpen = false;
	m_iIndex = 0;

	
}

void CUI_Inventory::Update_UI_Inventory()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWND, &pt);
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Instance()->Get_Scroll();
	pt.x -= vScroll.x;
	pt.y -= vScroll.y;
	
	
	if (CKey_Manager::Get_Instance()->Key_Down(KEY_LBUTTON))
	{
		CSoundMgr::Get_Instance()->PlaySound(L"Click0.wav", CSoundMgr::UI);
		auto it = m_vInventory.begin();
		int iSize = m_vInventory.size();

		if (PtInRect(&m_tRectInventory[0], pt) && iSize>0)
		{
			SelectedInven(it);
		}

		if (PtInRect(&m_tRectInventory[1], pt) && iSize>1)
		{
			it += 1;
			SelectedInven(it);
		}

		if (PtInRect(&m_tRectInventory[2], pt) && iSize>2)
		{
			it += 2;
			SelectedInven(it);
		}
		if (PtInRect(&m_tRectInventory[3], pt) && iSize>3)
		{
			it += 3;
			SelectedInven(it);
		}
		if (PtInRect(&m_tRectInventory[4], pt) && iSize>4)
		{
			it += 4;
			SelectedInven(it);
		}
		m_pTarget->Change_AnimState(m_pTarget->Get_Name(), L"Idle");

		CCombat_Manager::Get_Instance()->CloseUI();

		m_bIsOpen = false;
	}
}

void CUI_Inventory::Render_UI_Inventory()
{
	/// <summary>
	///  inventory 백그라운드
	/// </summary>

	CUI_Inventory::UpdateRect_GameObject();
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_Manager(L"UI", L"MenuUI", 2);

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


	////// 아이템 내용물
	int iSize = m_vInventory.size();
	for (int i = 0; i < iSize; ++i)
	{
		auto dwDrawId = dynamic_cast<CItem*>(m_vInventory[i])->Get_DrawId();

		const TEXINFO* pItemTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_Manager(L"Item", L"Item", dwDrawId);

		D3DXMATRIX matScale, matTrans, matWorld;

		float fOffsetX = 100;
		float fOffsetY = 62+20 + 63*i;

		float fCenterX = float(pItemTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pItemTexInfo->tImageInfo.Height >> 1);

		D3DXMatrixScaling(&matScale, 1.f , 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, fOffsetX + fCenterX,  fOffsetY, 0.f);
		matWorld = matScale * matTrans;

		if (nullptr == pTexInfo)
			return;
	

		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pItemTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	}
}

void CUI_Inventory::UpdateRect_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_Manager(L"UI", L"MenuUI", 2);

	float fScrollX = CScroll_Manager::Get_Instance()->Get_Scroll(CScroll_Manager::eSB_HORZ);
	float fScrollY = CScroll_Manager::Get_Instance()->Get_Scroll(CScroll_Manager::eSB_VERT);

	float fCenterX = 314 / 2;
	float fCenterY = 400 / 2;

	float fOffsetX = m_tInfo.vPos.x - fCenterX + 70 - 32;
	float fOffsetXright = m_tInfo.vPos.x - fCenterX + 70 + 280 -32;
	float fOffsetY = m_tInfo.vPos.y - fCenterY + 52 + 12;
	float fOffsetYbottom = m_tInfo.vPos.y - fCenterY + 52 + 63 + 12;

	int iSize = m_vInventory.size();
	for (int i = 0; i < iSize; i++)
	{
		float fSpaceY = 63 * i;

		m_tRectInventory[i].left = LONG(fOffsetX) - fScrollX;
		m_tRectInventory[i].top = static_cast<LONG>(fOffsetY + fSpaceY) - fScrollY;
		m_tRectInventory[i].right = static_cast<LONG>(fOffsetXright) - fScrollX;
		m_tRectInventory[i].bottom = static_cast<LONG>(fOffsetYbottom + fSpaceY) - fScrollY;
	}

}

void CUI_Inventory::ADD_InvenItem(CGameObject* pitem)
{
	if (m_vInventory.size() > 5)
		return;

	m_vInventory.push_back(pitem);
}

void CUI_Inventory::SelectedInven(vector<CGameObject*>::iterator iterInven)
{
	auto ItemDESC = dynamic_cast<CItem*>(*iterInven)->Get_ITEM_DESC();
	if (ItemDESC.eType == ITEM_TYPE::CONSUMABLE)
	{
		
		//auto ItemDESC = dynamic_cast<CItem*>(pObject)->Get_ITEM_DESC();

		m_pTarget->Set_Hp(ItemDESC.iTemCount1);
		m_pTarget->Add_Exp(ItemDESC.iTemCount2);

		float fScrollX = CScroll_Manager::Get_Instance()->Get_Scroll(CScroll_Manager::eSB_HORZ);
		float fScrollY = CScroll_Manager::Get_Instance()->Get_Scroll(CScroll_Manager::eSB_VERT);

		auto Pos = m_pTarget->Get_Info()->vPos ;

		Pos += {fScrollX, fScrollY, 0.f};

		if (ItemDESC.iTemCount1 > 0)
		{
			CEffect_Manager::Get_Instance()->AddDigitEffect(ItemDESC.iTemCount1, Pos, {0,255,0});
			CSoundMgr::Get_Instance()->PlaySound(L"Item0.wav", CSoundMgr::EFFECT);

		}
		else if (ItemDESC.iTemCount2 > 0)
		{
			CEffect_Manager::Get_Instance()->AddDigitEffect(ItemDESC.iTemCount2, Pos, { 255,0,0 });
			CSoundMgr::Get_Instance()->PlaySound(L"Item0.wav", CSoundMgr::EFFECT);
		}
		CUI_Manager::Get_Instance()->SelectUnit(m_pTarget);
		
		CGame_Manager::Get_Instance()->Delete_InvenItemKey(m_pTarget->Get_Name(), *iterInven);

		m_vInventory.erase(iterInven);
	}
	if (ItemDESC.eType == ITEM_TYPE::EQUIP)
	{
		//m_pTarget->SetAtk
	}

	m_pTarget->Set_AddUnitState(UNIT_ATTACK | UNIT_END);

	
	m_bIsOpen = false;
}

CGameObject* CUI_Inventory::Use_Delete_InvenItem(int iIndex)
{
	if (m_vInventory.size() < iIndex)
		return nullptr;

	auto iter = m_vInventory.begin();

	iter += iIndex - 1;

	return *(m_vInventory.erase(iter));
}
