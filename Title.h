#pragma once
#include "Scene.h"
class CTitle :
    public CScene
{
    // CScene��(��) ���� ��ӵ�
    virtual HRESULT Ready_Scene() override;
    virtual void Update_Scene() override;
    virtual void Render_Scene() override;
    virtual void Release_Scene() override;
private:
    float m_fTime;
    float m_fFrame;
};

