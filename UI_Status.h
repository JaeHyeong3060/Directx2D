#pragma once
#include "UI_Observer.h"
class CUI_Status :
    public CUI_Observer
{
    // CUI_Observer��(��) ���� ��ӵ�
    virtual void NotifySelectUnit(CGameObject* pObject) override;
    virtual void SelectedUnitMoveEnd(CGameObject* pObject) override;
private:
    CGameObject* pCurObject=nullptr;

 
};

