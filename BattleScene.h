#pragma once
#include "Scene.h"
class CGameObject_Manager;
class CBattleScene :
    public CScene
{
public:
    void Set_ObjSource(CGameObject* pObjSource) { m_pObjSource = pObjSource; }
    void Set_ObjDest(CGameObject* pObjDest) { m_pObjDest = pObjDest; }
public:
    // CScene을(를) 통해 상속됨
    virtual HRESULT Ready_Scene() override;
    virtual void Update_Scene() override;
    virtual void Render_Scene() override;
    virtual void Release_Scene() override;
public:
    // Source = Right / Dest = Left
    static CScene* Create(CGameObject* pObjectSource, CGameObject* pObjectDest);
private:
    CGameObject_Manager* m_pGameObject_Manager;
    CGameObject* m_pObjSource;
    CGameObject* m_pObjDest;
    DWORD m_dwStageNumber;
    ANIMCLIP* m_pAnimClipObjSource;
    ANIMCLIP* m_pAnimClipObjDest;
    INFO m_tSourceInfo;
    INFO m_tDestInfo;

    float m_fDelay;

    bool m_bSourceAnimEnd;
    bool m_bDestAnimEnd;

    float m_fEndTime;

};

