#pragma once
#include "UI_Object.h"
class CMyButton
	:public CUI_Object
{
private:
	CMyButton();
public:
	virtual ~CMyButton();
public:
	void Set_UI_DESC(UI_DESC* pDESC);
	void Set_FrameKey(const wstring& wstrFrameKey) { m_wstrFrameKey = wstrFrameKey; }

public:
	void CMyButton::UpdateRect_GameObject();
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
public:
	static CGameObject* Create(LPVOID* pParam=nullptr);
private:
	wstring m_wstrFrameKey;
	DWORD m_dwDrawID;

};

