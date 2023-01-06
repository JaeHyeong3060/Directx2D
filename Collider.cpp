#include "stdafx.h"
#include "Collider.h"
#include "GameObject.h"

CCollider::CCollider()
{
}

CCollider::~CCollider()
{
}

int CCollider::Ready_GameObject()
{
	return 0;
}

int CCollider::Update_GameObject()
{

	return 0;
}

void CCollider::Late_Update_GameObject()
{
}

void CCollider::Render_GameObject()
{


}

void CCollider::Release_GameObject()
{
}

CCollider* CCollider::Create()
{
	CCollider* pInstance = new CCollider;
	if (0 > pInstance->Ready_GameObject())
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}
