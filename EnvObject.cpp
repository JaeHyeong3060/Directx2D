#include "stdafx.h"
#include "EnvObject.h"


void CEnvObject::Set_DESC(UNIT_DESC* pDESC)
{
	m_wstrName = pDESC->wstr_name;
	m_tInfo = pDESC->tInfo;
//	m_tUnitInfo = pDESC->tUnitInfo;
	m_dwLocate = pDESC->dwLocate;
}

HRESULT CEnvObject::Ready_GameObject()
{
	m_HitObject = nullptr;
	m_bHitPlayer = false;
    return S_OK;
}

int CEnvObject::Update_GameObject()
{
	
	if (m_bHitPlayer == true)
	{
		if (m_wstrName == L"VilliageEnv0")
		{
			CDialog_Manager::Get_Instance()->Set_DialogIndex(ST_EVENT_Villiage_START, ST_EVENT_Villiage_END);
			CGame_Manager::Get_Instance()->Add_Gold(500);
		}
		if (m_wstrName == L"Exit0")
		{
			CDialog_Manager::Get_Instance()->Set_DialogIndex(ST_THIRD_DIALOG_START, ST_THIRD_DIALOG_END);
			CDialog_Manager::Get_Instance()->Set_NextSceneIndex(CScene_Manager::DIALOG);

			CScene_Manager::Get_Instance()->Change_Scene_Manager(CScene_Manager::DIALOG);
			::ShowWindow(g_hUIStatus, SW_HIDE);
		}
		if (m_wstrName == L"Shop0")
		{
			if(m_HitObject!=nullptr)
				CScene_Manager::Get_Instance()->Change_Shop_Scene(m_HitObject);
			m_HitObject = nullptr;
		}
		m_bIsDead = true;
		return OBJ_DEAD;
	}

    return OBJ_NOEVENT;
}

void CEnvObject::Late_Update_GameObject()
{
	if (m_bHitPlayer == false)
	{
		auto Playerlist = CGameObject_Manager::Get_Instance()->Get_List(PLAYER);

		auto itHit = find_if(Playerlist.begin(), Playerlist.end(), [this](CGameObject* pObject) {
			return ((pObject->Get_Locate() == this->Get_Locate()) && pObject->Get_Moving() == false);
			});

		if (itHit != Playerlist.end())
		{
			m_bHitPlayer = true;
			m_HitObject = (*itHit);
		}
	}
}

void CEnvObject::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_Manager(L"Terrain", L"Tile", 2);

	D3DXMATRIX matScale, matTrans, matWorld;

	if (nullptr == pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + CScroll_Manager::Get_Instance()->Get_Scroll(CScroll_Manager::eSB_HORZ), m_tInfo.vPos.y + CScroll_Manager::Get_Instance()->Get_Scroll(CScroll_Manager::eSB_VERT), 0.f);
	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(200, 255, 255, 255));

}

void CEnvObject::Release_GameObject()
{
}

CGameObject* CEnvObject::Create(LPVOID* pParam)
{
	CGameObject* pInstance = new CEnvObject;
	if (0 > pInstance->Ready_GameObject())
	{
		Safe_Delete(pInstance);
		return pInstance;
	}
	if (nullptr != pParam)
		dynamic_cast<CEnvObject*>(pInstance)->Set_DESC((UNIT_DESC*)pParam);

	return pInstance;
}