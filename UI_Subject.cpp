#include "stdafx.h"
#include "UI_Subject.h"
#include "GameObject.h"

CUI_Subject::CUI_Subject():
	m_vObservers({})
{
}

CUI_Subject::~CUI_Subject()
{
}

void CUI_Subject::Message(CGameObject* pObject)
{
	for (auto& it : m_vObservers)
	{
		it->NotifySelectUnit(pObject);
	}
}
