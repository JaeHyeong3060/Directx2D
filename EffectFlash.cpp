#include "stdafx.h"
#include "EffectFlash.h"

HRESULT CEffectFlash::Ready_GameObject()
{
	m_iAlpha = 255;

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BATTLE);
	CSoundMgr::Get_Instance()->PlaySound(L"Attack0.wav", CSoundMgr::BATTLE);

	return S_OK;
}

int CEffectFlash::Update_GameObject()
{
	if (m_bIsDead)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void CEffectFlash::Late_Update_GameObject()
{
}

void CEffectFlash::Render_GameObject()
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

		const TEXINFO* pFadeText = CTexture_Manager::Get_Instance()->Get_TexInfo_Manager(L"Title", L"TitleImg", 4);

		fCenterX = pFadeText->tImageInfo.Width >> 1;
		fCenterY = pFadeText->tImageInfo.Height >> 1;
		D3DXMatrixTranslation(&matTrans, fCenterX, fCenterY, 0.f);
		matWorld = matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pFadeText->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

		m_iAlpha -= CTime_Manager::Get_Instance()->Get_DeltaTime() * TIME_SCALE_EFFECT;
	}
}


void CEffectFlash::Release_GameObject()
{
}

CGameObject* CEffectFlash::Create()
{

	CGameObject* pInstance = new CEffectFlash;
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}