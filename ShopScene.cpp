#include "stdafx.h"
#include "Item.h"
#include "ShopScene.h"
#include "Player.h"

HRESULT CShopScene::Ready_Scene()
{
    //CCombat_Manager::Get_Instance()->Selected_Menu_Inventory(m_pTarget);

    //CDialog_Manager::Get_Instance()->Set_DialogIndex(ST_SHOP_START, ST_SHOP_END);

	CEffect_Manager::Get_Instance()->Set_StateEnd(false);

	CSoundMgr::Get_Instance()->StopAll();
	CSoundMgr::Get_Instance()->PlayBGM(L"55 Shops.mp3");


	ITEM_DESC* pitem_DESC = new ITEM_DESC;
	pitem_DESC->eType = CONSUMABLE;
	pitem_DESC->dwDrawId = 0;
	pitem_DESC->iTemCount1 = 10;
	pitem_DESC->iTemCount2 = 0;
	pitem_DESC->iTemCount3 = 0;
	pitem_DESC->price = 100;

	CGameObject* pObject = CItem::Create((LPVOID*)pitem_DESC);


	ITEM_DESC* pitem_DESCtwo = new ITEM_DESC;
	pitem_DESCtwo->eType = CONSUMABLE;
	pitem_DESCtwo->iTemCount3 = 0;
	pitem_DESCtwo->iTemCount1 = 0;
	pitem_DESCtwo->iTemCount2 = 50;
	pitem_DESCtwo->dwDrawId = 1;
	pitem_DESCtwo->price = 200;

	CGameObject* pObjectTwo = CItem::Create((LPVOID*)pitem_DESCtwo);

	m_vShoplist.push_back(pObject);
	m_vShoplist.push_back(pObjectTwo);
	m_vShoplist.push_back(pObject);
	m_vShoplist.push_back(pObjectTwo);
	m_vShoplist.push_back(pObject);

	
	m_iCurKey = 0;

	m_wstrDialog[0] = L"어서오세요 ";
	m_wstrDialog[1] = L"여기는 포션 가게 입니다.";
	m_wstrDialog[2] = L"어떤 포션을 드릴까요?";
	m_wstrDialog[3] = L"레드포션은 100골드, Exp포션은 200골드 입니다.";
	m_wstrDialog[4] = L"구입해주셔서 감사합니다.";
	m_wstrDialog[5] = L"죄송하지만 소지품이 가득차신 것 같네요";
	m_wstrDialog[6] = L"돈이 부족하시네요.";

	Set_Rect_GameObject();
    return S_OK;
}

void CShopScene::Update_Scene()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWND, &pt);
	/*D3DXVECTOR3 vScroll = CScroll_Manager::Get_Instance()->Get_Scroll();
	pt.x -= vScroll.x;
	pt.y -= vScroll.y;*/


	if (CKey_Manager::Get_Instance()->Key_Down(KEY_LBUTTON))
	{
		auto it = m_vShoplist.begin();
		int iSize = m_vShoplist.size();

		if (PtInRect(&m_ShopRect[0], pt) && iSize > 0)
		{
			printf("0");
			SelectedShop(it);
		}

		if (PtInRect(&m_ShopRect[1], pt) && iSize > 1)
		{
			printf("1");
			it += 1;
			SelectedShop(it);
		}

		if (PtInRect(&m_ShopRect[2], pt) && iSize > 2)
		{
			printf("2");
			it += 2;
			SelectedShop(it);
		}
		if (PtInRect(&m_ShopRect[3], pt) && iSize > 3)
		{
			printf("3");
			it += 3;
			SelectedShop(it);
		}
		if (PtInRect(&m_ShopRect[4], pt) && iSize > 4)
		{
			printf("4");
			it += 4;
			SelectedShop(it);
		}

		//CCombat_Manager::Get_Instance()->CloseUI();

		//m_bIsOpen = false;
	}
	if (CKey_Manager::Get_Instance()->Key_Down(KEY_X))
	{
		CScene_Manager::Get_Instance()->Return_Prev_Scene();
	}
	if (CKey_Manager::Get_Instance()->Key_Up(KEY_Z))
	{
		if(m_iCurKey<4)
			m_iCurKey++;
		if (m_iCurKey >= 4)
			m_iCurKey = 3;
	}
}

void CShopScene::Render_Scene()
{
    D3DXMATRIX matScale, matTrans, matWorld;

	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_Manager(L"Terrain", L"LocalMap", 4);

    D3DXMatrixScaling(&matScale, 1.0f, 1.0f, 0.f);
    D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);
    matWorld = matScale * matTrans;
    //float fCenterX = pTexInfo->tImageInfo.Width >> 1;
    //float fCenterY = pTexInfo->tImageInfo.Height >> 1;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);// 배경은 -8.f 만큼 보정해줌
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	wstring wstrGold;
	wstrGold = to_wstring(CGame_Manager::Get_Instance()->Get_Gold());
	D3DXMatrixTranslation(&matTrans, 660, 205, 0.f);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
	CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), wstrGold.c_str(), lstrlen(wstrGold.c_str()), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	auto CurDialog = m_wstrDialog[m_iCurKey].c_str();
	D3DXMatrixTranslation(&matTrans, 240, 80, 0.f);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
	CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), CurDialog, lstrlen(CurDialog), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));



	int iSize = m_vShoplist.size();
	for (int i = 0; i < iSize; ++i)
	{
		auto dwDrawId = dynamic_cast<CItem*>(m_vShoplist[i])->Get_DrawId();

		const TEXINFO* pItemTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_Manager(L"Item", L"Item", dwDrawId);

		D3DXMATRIX matScale, matTrans, matWorld;

		float fOffsetX = 187;
		float fOffsetY = 270 + 60 * i;

		float fCenterX = float(pItemTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pItemTexInfo->tImageInfo.Height >> 1);

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, fOffsetX+ fCenterX, fOffsetY+fCenterY, 0.f);
		matWorld = matScale * matTrans;

		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pItemTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	}

	if (CEffect_Manager::Get_Instance()->Get_StateEnd() == false)
	{
		CEffect_Manager::Get_Instance()->FadeOutEffect();
	}

	CEffect_Manager::Get_Instance()->Render_Effect_Manager();
}

void CShopScene::Release_Scene()
{
}

CScene* CShopScene::Create(CGameObject* pObject)
{
    CScene* pInstance = new CShopScene;
    dynamic_cast<CShopScene*>(pInstance)->Set_Target(pObject);
    if (FAILED(pInstance->Ready_Scene()))
    {
        delete pInstance;
        return nullptr;
    }
    return pInstance;
}

void CShopScene::Set_Rect_GameObject()
{
	float fScrollX = CScroll_Manager::Get_Instance()->Get_Scroll(CScroll_Manager::eSB_HORZ);
	float fScrollY = CScroll_Manager::Get_Instance()->Get_Scroll(CScroll_Manager::eSB_VERT);

	//float fCenterX = 314 / 2;
	//float fCenterY = 400 / 2;

	float fOffsetX = 187;
	float fOffsetXright = 667;
	float fOffsetY = 270;
	float fOffsetYbottom = 330;

	int iSize = m_vShoplist.size();
	for (int i = 0; i < iSize; i++)
	{
		float fSpaceY = 60 * i;

		m_ShopRect[i].left = static_cast<LONG>(fOffsetX);
		m_ShopRect[i].top = static_cast<LONG>(fOffsetY + fSpaceY);
		m_ShopRect[i].right = static_cast<LONG>(fOffsetXright);
		m_ShopRect[i].bottom = static_cast<LONG>(fOffsetYbottom + fSpaceY);
	}

}

void CShopScene::SelectedShop(vector<CGameObject*>::iterator iterShop)
{
	auto ItemDESC = dynamic_cast<CItem*>(*iterShop)->Get_ITEM_DESC();
	if (ItemDESC.eType == ITEM_TYPE::CONSUMABLE)
	{
		if (CGame_Manager::Get_Instance()->Get_Gold() >= ItemDESC.price)
		{
			if (CGame_Manager::Get_Instance()->Get_KeyInvenSize(m_pTarget->Get_Name()) < 5)
			{
				CGame_Manager::Get_Instance()->ADD_InvenItemKey(m_pTarget->Get_Name(), *iterShop);
				CGame_Manager::Get_Instance()->Add_Gold(-ItemDESC.price);
				m_vShoplist.erase(iterShop);
				m_iCurKey = 4;
				CSoundMgr::Get_Instance()->PlaySound(L"Shop0.wav", CSoundMgr::EFFECT);
			}
			else
				m_iCurKey = 5;
		}
		else
		{
			m_iCurKey = 6;
		}
		//auto ItemDESC = dynamic_cast<CItem*>(pObject)->Get_ITEM_DESC();

		//m_pTarget->Set_Hp(ItemDESC.iTemCount1);
		//m_pTarget->Add_Exp(ItemDESC.iTemCount2);

		//float fScrollX = CScroll_Manager::Get_Instance()->Get_Scroll(CScroll_Manager::eSB_HORZ);
		//float fScrollY = CScroll_Manager::Get_Instance()->Get_Scroll(CScroll_Manager::eSB_VERT);

		//auto Pos = m_pTarget->Get_Info()->vPos;

		//Pos += {fScrollX, fScrollY, 0.f};

		//if (ItemDESC.iTemCount1 > 0)
		//	CEffect_Manager::Get_Instance()->AddDigitEffect(ItemDESC.iTemCount1, Pos, { 0,255,0 });
		//else if (ItemDESC.iTemCount2 > 0)
		//	CEffect_Manager::Get_Instance()->AddDigitEffect(ItemDESC.iTemCount2, Pos, { 255,0,0 });

		//CUI_Manager::Get_Instance()->SelectUnit(m_pTarget);

	
		
	}
	if (ItemDESC.eType == ITEM_TYPE::EQUIP)
	{
		//m_pTarget->SetAtk
	}
	
	//m_pTarget->Set_AddUnitState(UNIT_ATTACK | UNIT_END);


	//m_bIsOpen = false;
}
