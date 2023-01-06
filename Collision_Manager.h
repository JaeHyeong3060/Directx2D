#pragma once
class CGameObject;
class CCollider;
class CCollision_Manager
	//Collision

{
public:
	DECLARE_SINGLETON(CCollision_Manager)
private:
	CCollision_Manager();
	~CCollision_Manager();
public:
	//void Collision(float fDeltaTime);
public:
	static void Collision_Rect(list<CGameObject*>* plistDest, list<CGameObject*>* plistSrc);
private:
	list<CGameObject*>	m_CollisionList;
};

