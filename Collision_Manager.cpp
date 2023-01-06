#include "stdafx.h"
#include "Collision_Manager.h"
#include "GameObject.h"
#include "Player.h"
#include "Collider.h"

IMPLEMENT_SINGLETON(CCollision_Manager)
CCollision_Manager::CCollision_Manager()
{
}


CCollision_Manager::~CCollision_Manager()
{
}

//void CCollision_Manager::Collision(float fDeltaTime)
//{
//	if (m_CollisionList.size() < 2)
//	{
//		m_CollisionList.clear();
//		return;
//	}
//
//	m_CollisionList.clear();
//}

void CCollision_Manager::Collision_Rect(list<CGameObject*>* plistDest, list<CGameObject*>* plistSrc)
{
	RECT rc = {};
	for (auto& pDstObject : *plistDest)
	{
		for (auto& pSrcObject : *plistSrc)
		{
			if (IntersectRect(&rc, pDstObject->Get_Rect(), pSrcObject->Get_Rect()))
			{
				pSrcObject->Set_Dead(true);
			}
		}
	}
}

