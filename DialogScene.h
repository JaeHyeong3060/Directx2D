#pragma once
#include "Scene.h"
#include "GameObject.h"

class CGameObject;

class CDialogScene :
    public CScene
{
    // CScene��(��) ���� ��ӵ�
    virtual HRESULT Ready_Scene() override;
    virtual void Update_Scene() override;
    virtual void Render_Scene() override;
    virtual void Release_Scene() override;

private:
    DWORD m_dwAlpha=255;

};

