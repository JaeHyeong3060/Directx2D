#include "stdafx.h"
#include "Effect.h"
#include "Effect_Digit.h"
#include "EffectFlash.h"
#include "Effect_Manager.h"

IMPLEMENT_SINGLETON(CEffect_Manager)

CEffect_Manager::CEffect_Manager()
{
}

CEffect_Manager::~CEffect_Manager()
{
}

void CEffect_Manager::Ready_Effect_Manager()
{
	m_bIsEffect = false;
	m_iAlpha = 255;
}

void CEffect_Manager::FadeOutEffect()
{
	{
		D3DXMATRIX matTrans, matScale, matWorld;
		float fCenterX = 0.f;
		float fCenterY = 0.f;

		const TEXINFO* pFadeText = CTexture_Manager::Get_Instance()->Get_TexInfo_Manager(L"Title", L"TitleImg", 3);

		fCenterX = pFadeText->tImageInfo.Width >> 1;
		fCenterY = pFadeText->tImageInfo.Height >> 1;
		D3DXMatrixTranslation(&matTrans, fCenterX, fCenterY, 0.f);
		matWorld = matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pFadeText->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
		
		m_iAlpha -= CTime_Manager::Get_Instance()->Get_DeltaTime() * TIME_SCALE_EFFECT;
	}
	if (m_iAlpha <= 0)
	{
		m_iAlpha = 255;
		m_bIsEffect = true;
	}
}



void CEffect_Manager::AddDigitEffect(int number,D3DVECTOR& pos,RGBTRIPLE RGB)
{
	CGameObject* pObject = CEffect_Digit::Create();
	dynamic_cast<CEffect_Digit*>(pObject)->Set_Number(number);
	dynamic_cast<CEffect_Digit*>(pObject)->Set_Pos(pos);
	dynamic_cast<CEffect_Digit*>(pObject)->Set_RGB(RGB);

	Add_Effect_Manager(pObject);
}

void CEffect_Manager::Add_Etc_Effect(D3DVECTOR& pos, DWORD dwDrawId)
{
	CGameObject* pObject = CEffect::Create();
	dynamic_cast<CEffect*>(pObject)->Set_Pos(pos);
	dynamic_cast<CEffect*>(pObject)->Set_DrawId(dwDrawId);

	Add_Effect_Manager(pObject);
}

void CEffect_Manager::Update_Effect_Manager()
{
	for (auto& iter = m_listEffectObject.begin(); iter != m_listEffectObject.end();)
	{
		int iEvent = (*iter)->Update_GameObject();
		if (OBJ_DEAD == iEvent)
		{
			Safe_Delete(*iter);
			iter = m_listEffectObject.erase(iter);
		}
		else
			++iter;
	}
	/*for (auto& pGameObject : m_listEffectObject)
	{
		pGameObject->Late_Update_GameObject();
	}*/
}

void CEffect_Manager::Render_Effect_Manager()
{
	for (auto& pGameObject : m_listEffectObject)
	{
		pGameObject->Render_GameObject();
	}
}

void CEffect_Manager::Clear_Effect_List()
{
	while (!m_listEffectObject.empty())
	{
		m_listEffectObject.pop_back();
	}
	m_listEffectObject.clear();
}

void CEffect_Manager::FlashEffect()
{
	CGameObject* pObject = CEffectFlash::Create();
	
	Add_Effect_Manager(pObject);
}
