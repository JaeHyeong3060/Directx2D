#pragma once
class CGameObject;
class CUI_Observer abstract
{

public:
	virtual void NotifySelectUnit(CGameObject* pObject) PURE;
	virtual void SelectedUnitMoveEnd(CGameObject* pObject) PURE;
};

