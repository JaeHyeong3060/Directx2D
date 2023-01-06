#pragma once
#include "Scene.h"
class CShopScene :
    public CScene
{
public:
    void Set_Target(CGameObject* pTarget) { m_pTarget = pTarget; }
public:
    // CScene을(를) 통해 상속됨
    virtual HRESULT Ready_Scene() override;
    virtual void Update_Scene() override;
    virtual void Render_Scene() override;
    virtual void Release_Scene() override;
public:
    void SelectedShop(vector<CGameObject*>::iterator iterShop);
    void Set_Rect_GameObject();
public:
    static CScene* Create(CGameObject* pObject);
private:
    CGameObject* m_pTarget;

    int m_iCurKey;
    wstring m_wstrDialog[10]{};

    vector<CGameObject*> m_vShoplist;
    RECT m_ShopRect[5];
};