#pragma once
#include "GameObject.h"
class CEnvObject :
    public CGameObject
{
public:
    void Set_DESC(UNIT_DESC* pDESC);
public:
    // CGameObject을(를) 통해 상속됨
    virtual HRESULT Ready_GameObject() override;
    virtual int Update_GameObject() override;
    virtual void Late_Update_GameObject() override;
    virtual void Render_GameObject() override;
    virtual void Release_GameObject() override;
public:
    static CGameObject* Create(LPVOID* pParam = nullptr);
private:
    bool m_bHitPlayer = false;
    CGameObject* m_HitObject = nullptr;
};