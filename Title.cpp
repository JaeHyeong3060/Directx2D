#include "stdafx.h"
#include "Title.h"

HRESULT CTitle::Ready_Scene()
{
	m_fTime = 5.f;
	m_fFrame = 0.f;
	return S_OK;
}

void CTitle::Update_Scene()
{
	if (m_fFrame < 2.f)
		m_fFrame += CTime_Manager::Get_Instance()->Get_DeltaTime();
	else if (m_fFrame > 2.f)
		m_fFrame = 2.f;

	if (CKey_Manager::Get_Instance()->Key_Up(KEY_RETURN))
	{
		CScene_Manager::Get_Instance()->Change_Scene_Manager(CScene_Manager::DIALOG);
	}
}

void CTitle::Render_Scene()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_Manager(L"Title", L"TitleImg", m_fFrame);

	D3DXMATRIX matScale, matTrans, matWorld;

	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	D3DXMatrixTranslation(&matTrans, fCenterX, fCenterY, 0.f);
	matWorld = matTrans;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CTitle::Release_Scene()
{

}
