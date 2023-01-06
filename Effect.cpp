#include "stdafx.h"
#include "Effect.h"

HRESULT CEffect::Ready_GameObject()
{
	m_iAlpha = 255;
	m_dwDrawId = 0;

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::SYSTME);
	CSoundMgr::Get_Instance()->PlaySound(L"LevelUp.wav",CSoundMgr::SYSTME);
	return S_OK;
}

int CEffect::Update_GameObject()
{
	if (m_bIsDead)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void CEffect::Late_Update_GameObject()
{
}

void CEffect::Render_GameObject()
{
	if (m_iAlpha <= 1)
	{
		m_iAlpha = 0;
		m_bIsDead = true;
	}
	{
		D3DXMATRIX matTrans, matScale, matWorld;
		float fCenterX = 0.f;
		float fCenterY = 0.f;

		const TEXINFO* pFadeText = CTexture_Manager::Get_Instance()->Get_TexInfo_Manager(L"Effect", L"Effect", m_dwDrawId);

		fCenterX = pFadeText->tImageInfo.Width >> 1;
		fCenterY = pFadeText->tImageInfo.Height >> 1;
		D3DXMatrixScaling(&matScale, 3.f, 3.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, 0.f);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pFadeText->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

		m_iAlpha -= CTime_Manager::Get_Instance()->Get_DeltaTime();
	}
}


void CEffect::Release_GameObject()
{
}

CGameObject* CEffect::Create()
{
	CGameObject* pInstance = new CEffect;
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}