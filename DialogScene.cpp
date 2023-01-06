#include "stdafx.h"
#include "DialogScene.h"
#include "GameObject.h"


HRESULT CDialogScene::Ready_Scene()
{
	CEffect_Manager::Get_Instance()->Set_StateEnd(false);

	CSoundMgr::Get_Instance()->StopAll();
	CSoundMgr::Get_Instance()->PlayBGM(L"04 The Beginning.mp3");

	if (CDialog_Manager::Get_Instance()->Get_RecentDialogEnd() == 0)
	{
		CDialog_Manager::Get_Instance()->Set_DialogIndex(ST_BEGIN_START, ST_BEGIN_END);
		CDialog_Manager::Get_Instance()->Set_NextSceneIndex(CScene_Manager::STAGEA);
		return S_OK;
	}


	if (CDialog_Manager::Get_Instance()->Get_RecentDialogEnd() == ST_SECOND_DIALOG_END)
	{
		CDialog_Manager::Get_Instance()->Set_NextSceneIndex(CScene_Manager::STAGEB);
		return S_OK;
	}

	if (CDialog_Manager::Get_Instance()->Get_RecentDialogEnd() == ST_THIRD_DIALOG_END)
	{
		CDialog_Manager::Get_Instance()->Set_NextSceneIndex(CScene_Manager::STAGEC);
		return S_OK;
	}




	return S_OK;
}

void CDialogScene::Update_Scene()
{
	
}

void CDialogScene::Render_Scene()
{
	D3DXMATRIX matTrans, matScale, matWorld;
	float fCenterX = 0.f;
	float fCenterY = 0.f;

	const TEXINFO* pBackgroundInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_Manager(L"Terrain", L"LocalMap", 15);

	fCenterX = pBackgroundInfo->tImageInfo.Width >> 1;
	fCenterY = pBackgroundInfo->tImageInfo.Height >> 1;
	D3DXMatrixTranslation(&matTrans, fCenterX, fCenterY, 0.f);
	matWorld = matTrans;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pBackgroundInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (CEffect_Manager::Get_Instance()->Get_StateEnd() == false)
	{
		CEffect_Manager::Get_Instance()->FadeOutEffect();
	}

	CEffect_Manager::Get_Instance()->Render_Effect_Manager();
}

void CDialogScene::Release_Scene()
{
}
