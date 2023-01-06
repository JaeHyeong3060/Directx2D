#pragma once
#include "GameObject.h"
class CEffect_Digit :
    public CGameObject
{
public:
    void Set_Number(int number);
    void Set_Pos(D3DVECTOR& vPos) { m_vPos = vPos; };
    void Set_RGB(RGBTRIPLE& RGB) { m_RGB = RGB; }
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
    vector<const TEXINFO*> m_vDigitTexture;
    vector<BYTE> m_vDigit;
    D3DVECTOR m_vPos;
    int m_iAlpha;

    RGBTRIPLE m_RGB;
};

