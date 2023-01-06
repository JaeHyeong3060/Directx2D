#pragma once

#include"UI_Observer.h"
class CUI_Subject
{
public:
	CUI_Subject();
	~CUI_Subject();
public:
	void AddObserver(CUI_Observer* observer)
	{ 
		m_vObservers.push_back(observer);
	}
	void RemoveObserver(CUI_Observer* observer)
	{ 
		auto it = find(m_vObservers.begin(), m_vObservers.end(),observer);
		if (it != m_vObservers.end())
		{
			m_vObservers.erase(it);
		}
	}
	void Message(CGameObject* pObject);

private:
	vector<CUI_Observer*> m_vObservers;
};

