#pragma once
#include "GameObject.h"
class CEffect :
    public CGameObject
{
public:
    void Set_DrawId(DWORD dwid) { m_dwDrawId = dwid; };
    void Set_Pos(D3DVECTOR& vPos) { m_vPos = vPos; };
public:
    // CGameObject을(를) 통해 상속됨
    virtual HRESULT Ready_GameObject() override;
    virtual int Update_GameObject() override;
    virtual void Late_Update_GameObject() override;
    virtual void Render_GameObject() override;
    virtual void Release_GameObject() override;
public:
    static CGameObject* Create();
private:
    int m_iAlpha;
    DWORD m_dwDrawId;
    D3DVECTOR m_vPos;
};

