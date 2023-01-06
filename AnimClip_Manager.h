#pragma once
class CAnimClip_Manager
{
	DECLARE_SINGLETON(CAnimClip_Manager)
public:
	ANIMCLIP* Get_Clip(const wstring& wstrObjectKey, const wstring& wstrStateKey);

	void Add_Clip(const wstring& wstrObjectKey, const wstring& wstrStateKey, const DWORD& dwStartFrame,
		const DWORD& dwEndFrame, ANIMATION_OPTION eAnimOption, float fAnimTime, float fAnimLimitTime,float fEffectFrame = 0);


public:
	void ClipFrameMove(ANIMCLIP* AnimClip, float fSpeed = 1.f);

private:
	map<wstring, vector<ANIMCLIP*>> m_mapAnimClip;
};

