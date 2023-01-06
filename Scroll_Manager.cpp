#include "stdafx.h"
#include "Scroll_Manager.h"

IMPLEMENT_SINGLETON(CScroll_Manager)
CScroll_Manager::CScroll_Manager()
{
	ZeroMemory(&m_vScroll, sizeof(D3DXVECTOR3)); 
}


CScroll_Manager::~CScroll_Manager()
{
}

D3DXVECTOR3 CScroll_Manager::Get_Scroll()
{
	return m_vScroll;
}

float CScroll_Manager::Get_Scroll(ID eScrollBar)
{
	switch (eScrollBar)
	{
	case CScroll_Manager::eSB_HORZ:
		return m_vScroll.x; 
	case CScroll_Manager::eSB_VERT:
		return m_vScroll.y; 
	}
	return 0.f;
}

void CScroll_Manager::Set_Scroll(const D3DXVECTOR3 & vScroll)
{
	bool check = false;
	auto eCurScene = CScene_Manager::Get_Instance()->Get_CurScene();
	if (eCurScene == CScene_Manager::STAGEA)
	{
		if (m_vScroll.x + vScroll.x < -192)
		{
			m_vScroll.x = -192;
			check = true;
		}
		if (m_vScroll.x + vScroll.x > -32)
		{
			m_vScroll.x = -32;
			check = true;
		}
		if (m_vScroll.y + vScroll.y < -392)
		{
			m_vScroll.y = -392;
			check = true;
		}
		if (m_vScroll.y + vScroll.y > -32)
		{
			m_vScroll.y = -32;
			check = true;
		}
	}
	if (eCurScene == CScene_Manager::STAGEB)
	{
		if (m_vScroll.x + vScroll.x < -192)
		{
			m_vScroll.x = -192;
			check = true;
		}
		if (m_vScroll.x + vScroll.x > -32)
		{
			m_vScroll.x = -32;
			check = true;
		}
		if (m_vScroll.y + vScroll.y < -584)
		{
			m_vScroll.y = -584;
			check = true;
		}
		if (m_vScroll.y + vScroll.y > -32)
		{
			m_vScroll.y = -32;
			check = true;
		}
	}
	if (eCurScene == CScene_Manager::STAGEC)
	{
		if (m_vScroll.x + vScroll.x < -320)
		{
			m_vScroll.x = -320;
			check = true;
		}
		if (m_vScroll.x + vScroll.x > -32)
		{
			m_vScroll.x = -32;
			check = true;
		}
		if (m_vScroll.y + vScroll.y < -840)
		{
			m_vScroll.y = -840;
			check = true;
		}
		if (m_vScroll.y + vScroll.y > -32)
		{
			m_vScroll.y = -32;
			check = true;
		}
	}
	if(check == false)
		m_vScroll += vScroll;
	//printf("%f, %f\n", m_vScroll.x, m_vScroll.y);
}

void CScroll_Manager::Set_Scroll(ID eScrollBar, const float & fScroll)
{
	switch (eScrollBar)
	{
	case CScroll_Manager::eSB_HORZ:
		
		m_vScroll.x += fScroll; 
		return; 
	case CScroll_Manager::eSB_VERT:
		
		m_vScroll.y += fScroll; 
		return; 
	}
}

void CScroll_Manager::Move_Scroll_UnitPos(DWORD dwLocate)
{
	D3DVECTOR VecTemp;

	float PosX = ((dwLocate % TILEX) * TILECX);
	float PosY = ((dwLocate / TILEX) * TILECY);

	VecTemp = { 0.f, 0.f, 0.f };

	m_vScroll.x = 400.f - PosX;
	m_vScroll.y = 300.f - PosY;

	Set_Scroll(VecTemp);
}
