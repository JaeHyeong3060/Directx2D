#pragma once
class CEffect_Manager
{
	DECLARE_SINGLETON(CEffect_Manager)
public:
	CEffect_Manager();
	~CEffect_Manager();
public:
	bool Get_StateEnd() { return m_bIsEffect; }
	void Set_StateEnd(bool state) { m_bIsEffect = state; }
public:
	void AddDigitEffect(int number, D3DVECTOR& pos, RGBTRIPLE RGB = { 255,255,255 });
	void Add_Effect_Manager(CGameObject* pObject) { m_listEffectObject.push_back(pObject); }

	void Add_Etc_Effect(D3DVECTOR& pos,DWORD dwDrawId);

	void Clear_Effect_List();
	void FadeOutEffect();
	void FlashEffect();

public:
	void Ready_Effect_Manager();
	void Update_Effect_Manager();
	void Render_Effect_Manager();
private:
	list<CGameObject*> m_listEffectObject;
	int m_iAlpha;
	bool m_bIsEffect;
};

