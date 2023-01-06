#pragma once
#include "Scene.h"
class CStageC :
    public CScene
{
public:
	explicit CStageC();
	virtual ~CStageC();
public:
	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;
private:
	CGameObject_Manager* m_pGameObject_Manager;
};

