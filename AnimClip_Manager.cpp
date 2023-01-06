#include "stdafx.h"
#include "AnimClip_Manager.h"

IMPLEMENT_SINGLETON(CAnimClip_Manager)
//typedef struct _tagAnimClip
//{
//	wstring wstrObjectKey;
//	wstring wstrStateKey;
//	ANIMATION_OPTION eOption;
//	vector<TEXINFO*> vecTex;
//	float fFrameStart;
//	float fFrameEnd;
//	float fAnimTime;
//	float fAnimLimitTime;
//}ANIMCLIP;

ANIMCLIP* CAnimClip_Manager::Get_Clip(const wstring& wstrObjectKey, const wstring& wstrStateKey)
{
	// map에서 오브젝트 키 먼저 검색
	auto iter_find = m_mapAnimClip.find(wstrObjectKey);
	if (iter_find == m_mapAnimClip.end())
		return nullptr;

	// ANIMCLIP 에서 검색 state키 같은 것
	auto AnimClipList = iter_find->second;
	
	for (auto& it : AnimClipList)
	{
		if (it->wstrStateKey == wstrStateKey)
		{
			return it;
		}
	}
	return nullptr;
}


void CAnimClip_Manager::Add_Clip(const wstring& wstrObjectKey, const wstring& wstrStateKey, const DWORD& dwStartFrame,
	const DWORD& dwEndFrame,ANIMATION_OPTION eAnimOption,float fAnimTime,float fAnimLimitTime, float fEffectFrame)
{
	ANIMCLIP* pAnimClip = new ANIMCLIP;
	for (int i = dwStartFrame; i < dwEndFrame; i++)
	{
		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_Manager(wstrObjectKey, wstrStateKey, i);
		if (nullptr == pTexInfo)
			return;
		auto k = const_cast<TEXINFO*>(pTexInfo);
		pAnimClip->vecTex.emplace_back(k);
	}
	pAnimClip->wstrObjectKey = wstrObjectKey;
	pAnimClip->wstrStateKey = wstrStateKey;
	pAnimClip->fFrameStart = dwStartFrame;
	pAnimClip->fFrameEnd = dwEndFrame;
	pAnimClip->eOption = eAnimOption;
	pAnimClip->fAnimTime = fAnimTime;
	pAnimClip->fAnimLimitTime = fAnimLimitTime;
	pAnimClip->bClipEnd = false;
	pAnimClip->bEffectEnd = false;
	pAnimClip->fEffectFrame = fEffectFrame;
	m_mapAnimClip[wstrObjectKey].emplace_back(pAnimClip);
}

void CAnimClip_Manager::ClipFrameMove(ANIMCLIP* AnimClip, float fSpeed)
{
	ANIMATION_OPTION eOption = AnimClip->eOption;
	switch (eOption)
	{
	case AO_LOOP:
		AnimClip->fFrameStart += AnimClip->fFrameEnd * CTime_Manager::Get_Instance()->Get_DeltaTime() * fSpeed;
		if (AnimClip->fFrameEnd < AnimClip->fFrameStart)
			AnimClip->fFrameStart = 0.f;
		break;
	case AO_ONCE_RETURN:
		if(!AnimClip->bClipEnd)
			AnimClip->fFrameStart += AnimClip->fFrameEnd * CTime_Manager::Get_Instance()->Get_DeltaTime() * fSpeed;
		if (AnimClip->fEffectFrame < AnimClip->fFrameStart && AnimClip->bEffectEnd==false)
		{
			CEffect_Manager::Get_Instance()->FlashEffect();
			AnimClip->bEffectEnd = true;

		}
		if ((AnimClip->fFrameEnd < AnimClip->fFrameStart) && !AnimClip->bClipEnd)
		{
			AnimClip->bClipEnd = true;
			AnimClip->bEffectEnd = false;
			AnimClip->fFrameStart = 0.f;
		}
		break;
	case AO_ONCE_DESTROY:
		break;
	case AO_TIME_RETURN:
		break;
	case AO_TIME_DESTROY:
		break;
	default:
		break;
	}
}
