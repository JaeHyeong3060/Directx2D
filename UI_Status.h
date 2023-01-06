#pragma once
#include "UI_Observer.h"
class CUI_Status :
    public CUI_Observer
{
    // CUI_Observer을(를) 통해 상속됨
    virtual void NotifySelectUnit(CGameObject* pObject) override;
    virtual void SelectedUnitMoveEnd(CGameObject* pObject) override;
private:
    CGameObject* pCurObject=nullptr;

 
};

