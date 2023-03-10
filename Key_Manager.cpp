#include "stdafx.h"
#include "Key_Manager.h"

IMPLEMENT_SINGLETON(CKey_Manager)

CKey_Manager::CKey_Manager()
	:m_dwKey(0)
	, m_dwKeyDown(0)
	, m_dwKeyUP(0)
	, m_bIsKeyLock(false)
{
}


CKey_Manager::~CKey_Manager()
{
}

void CKey_Manager::Update_Key_Manager()
{
	if (m_bIsKeyLock)
		return;
	m_dwKey = 0;
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_dwKey |= KEY_LBUTTON;
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		m_dwKey |= KEY_RBUTTON;
	if (GetAsyncKeyState(VK_UP) & 0x8000)
		m_dwKey |= KEY_UP;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		m_dwKey |= KEY_DOWN;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_dwKey |= KEY_LEFT;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_dwKey |= KEY_RIGHT;
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		m_dwKey |= KEY_RETURN;
	if (GetAsyncKeyState('Z') & 0x8000)
		m_dwKey |= KEY_Z;
	if (GetAsyncKeyState('X') & 0x8000)
		m_dwKey |= KEY_X;
	if (GetAsyncKeyState('P') & 0x8000)
		m_dwKey |= KEY_P;
}

bool CKey_Manager::Key_Up(DWORD dwKey)
{
	if (m_dwKey & dwKey)
	{
		m_dwKeyUP |= dwKey;
		return false;
	}
	else if (m_dwKeyUP & dwKey)
	{
		m_dwKeyUP ^= dwKey;
		return true;
	}
	return false;
}

bool CKey_Manager::Key_Down(DWORD dwKey)
{
	//0000 0000
	//0000 0001
	//0000 0000 
	if ((m_dwKey & dwKey) && !(m_dwKeyDown & dwKey))
	{
		m_dwKeyDown |= dwKey;
		return true;
	}
	else if (!(m_dwKey & dwKey) && (m_dwKeyDown & dwKey))
	{
		m_dwKeyDown ^= dwKey;
		return false;
	}
	return false;
}

bool CKey_Manager::Key_Pressing(DWORD dwKey)
{
	if (m_dwKey & dwKey)
		return true;
	return false;
}
