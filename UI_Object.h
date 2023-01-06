#pragma once
#include "GameObject.h"
class CUI_Object :
    public CGameObject
{
public:
    UI_OPTION Get_UO() { return m_eUO; }
protected:
    UI_OPTION m_eUO;
};

