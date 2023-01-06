#pragma once
#include "GameObject.h"

struct ITEM_DESC
{
    ITEM_TYPE eType;
    wstring wstrKey;
    DWORD dwDrawId;
    int iTemCount1;
    int iTemCount2;
    int iTemCount3;
    float fPos;
    int price;
};
class CItem :
    public CGameObject
{
public:
    void Set_DESC(ITEM_DESC* pDESC);
    DWORD Get_DrawId() { return m_tItemDESC.dwDrawId; }

    ITEM_DESC& Get_ITEM_DESC() { return m_tItemDESC; }
public:
    // CGameObject을(를) 통해 상속됨
    virtual HRESULT Ready_GameObject() override;
    virtual int Update_GameObject() override;
    virtual void Late_Update_GameObject() override;
    virtual void Render_GameObject() override;
    virtual void Release_GameObject() override;
public:
    static CGameObject* Create(LPVOID* pParam = nullptr);

    ITEM_DESC m_tItemDESC;
};

