#pragma once
class CCollider
{
protected:
	CCollider();
	virtual ~CCollider();
public:
	COLLIDER::Collder_TYPE GetColliderType() const
	{
		return m_eCollType;
	}

	class CGameObject* GetObj() const
	{
		return m_pObj;
	}
public:
	void SetObj(class CGameObject* pObj)
	{
		m_pObj = pObj;
	}

public:
	virtual int Ready_GameObject();
	virtual int Update_GameObject();
	virtual void Late_Update_GameObject();
	virtual void Render_GameObject();
	virtual void Release_GameObject();
public:
	static CCollider* Create();
protected:
	COLLIDER::Collder_TYPE m_eCollType;
	class CGameObject* m_pObj;

};

