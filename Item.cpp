#include "stdafx.h"
#include "Item.h"

void CItem::Set_DESC(ITEM_DESC* pDESC)
{
	m_tItemDESC.eType = pDESC->eType;
	m_tItemDESC.dwDrawId = pDESC->dwDrawId;
	m_tItemDESC.wstrKey = pDESC->wstrKey;
	m_tItemDESC.iTemCount1 = pDESC->iTemCount1;
	m_tItemDESC.iTemCount2 = pDESC->iTemCount2;
	m_tItemDESC.iTemCount3 = pDESC->iTemCount3;
	m_tItemDESC.fPos = pDESC->fPos;
	m_tItemDESC.price = pDESC->price;

	m_tInfo.vPos = { m_tItemDESC.fPos ,m_tItemDESC.fPos ,0.f };
}

HRESULT CItem::Ready_GameObject()
{
	m_tInfo.vDir = { 0.f,0.f,0.f };
	m_tInfo.vSize = { 1.f,1.f,1.f };
    return S_OK;
}

int CItem::Update_GameObject()
{
    return 0;
}

void CItem::Late_Update_GameObject()
{
}

void CItem::Render_GameObject()
{/*
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_Manager(L"Item", L"Item", m_tItemDESC.dwDrawId);

	D3DXMATRIX matScale, matTrans, matWorld;
	
	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);

	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	matWorld = matScale * matTrans;

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));*/

}

void CItem::Release_GameObject()
{
}

CGameObject* CItem::Create(LPVOID* pParam)
{
	CGameObject* pInstance = new CItem;
	if (0 > pInstance->Ready_GameObject())
	{
		Safe_Delete(pInstance);
		return pInstance;
	}
	if (nullptr != pParam)
		dynamic_cast<CItem*>(pInstance)->Set_DESC((ITEM_DESC*)pParam);

	return pInstance;
}
