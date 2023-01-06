#include "stdafx.h"
#include "UI_Manager.h"
#include "UI_Subject.h"
#include "UI_Observer.h"
#include "UI_Status.h"
#include "UI_UnitMenu.h"
#include "UI_Inventory.h"
#include "MyButton.h"

IMPLEMENT_SINGLETON(CUI_Manager)

HRESULT CUI_Manager::Ready_UI_Manager()
{
	m_pUI_Subject = new CUI_Subject;
	m_pUI_Status = new CUI_Status;

	m_pUI_Subject->AddObserver(m_pUI_Status);


	m_fLoadingTime = 1.f;

	UI_DESC tUI_Desc;
	tUI_Desc.tUI_Info.vPos = { 700.f, 550.f, 0.f };
	tUI_Desc.tUI_Info.vDir = { 1.f, 1.f, 0.f };
	tUI_Desc.tUI_Info.vSize = { 1.f, 1.f, 0.f };
	tUI_Desc.wstrFrameKey = L"TurnEndBtn";
	tUI_Desc.dwDrawID = 0;
	CGameObject* pObject = CMyButton::Create((LPVOID*)&tUI_Desc);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(UI, pObject);

	m_iAlpha = 255;


	m_pUI_Inventory = new CUI_Inventory;
	m_pUI_Inventory->Ready_UI_Inventory();

    return S_OK;
}

void CUI_Manager::Update_UI_Manager()
{
}

void CUI_Manager::Update_UI_Manager_Stage()
{
	if(m_pUI_Inventory->Get_state())
		m_pUI_Inventory->Update_UI_Inventory();
}

void CUI_Manager::Render_UI_Manager_Loading()
{
	if (CCombat_Manager::Get_Instance()->Get_Loading())
	{
		m_iAlpha -= CTime_Manager::Get_Instance()->Get_DeltaTime() * TIME_SCALE_EFFECT;

		//m_fLoadingTime -= CTime_Manager::Get_Instance()->Get_DeltaTime() * TIME_SCALE_EFFECT;
		if (m_iAlpha <= 0)
		{
			m_fLoadingTime = 1.f;
			m_iAlpha = 255;
			CCombat_Manager::Get_Instance()->Set_Loading(false);
			return;
		}

		auto eCurturn = CCombat_Manager::Get_Instance()->Get_CurTurn();
		D3DXMATRIX matScale, matTrans, matWorld;

		D3DXMatrixScaling(&matScale, 1.0f, 1.0f, 0.f);
		D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);
		matWorld = matScale * matTrans;
		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_Manager(L"TurnUI", L"TurnUI", eCurturn);

		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(0, 0, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

	}
}

void CUI_Manager::Render_UI_Manager_Stage()
{
	CGameObject_Manager::Get_Instance()->Render_GameObject_Manager_UI();

	if (m_pUI_Inventory->Get_state())
	{
		m_pUI_Inventory->Render_UI_Inventory();
	}
}

void CUI_Manager::SelectUnit(CGameObject* pObject)
{
	m_pUI_Subject->Message(pObject);
}

void CUI_Manager::CreateUnitMenu(CGameObject* pObject)
{

	UI_DESC tUI_Desc;
	tUI_Desc.tUI_Info.vPos = { 700.f, 350.f, 0.f };
	tUI_Desc.tUI_Info.vDir = { 1.f, 1.f, 0.f };
	tUI_Desc.tUI_Info.vSize = { 1.f, 1.f, 0.f };
	tUI_Desc.wstrFrameKey = L"UnitMenu";
	tUI_Desc.dwDrawID = 0;
	tUI_Desc.dwUnitState = pObject->Get_UnitState();

	auto pMenuObject = CUI_UnitMenu::Create((LPVOID*)&tUI_Desc);
	pMenuObject->Set_Target(pObject);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(UI, pMenuObject);
}

void CUI_Manager::CreateInventory(CGameObject* pObject)
{
	m_pUI_Inventory->Set_state(true);
	m_pUI_Inventory->Set_Target(pObject);
	m_pUI_Inventory->Set_Index(pObject->Get_Name());
}
