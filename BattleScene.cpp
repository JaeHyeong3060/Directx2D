#include "stdafx.h"
#include "Terrain.h"
#include "BattleScene.h"
#include "GameObject.h"

HRESULT CBattleScene::Ready_Scene()
{

	CEffect_Manager::Get_Instance()->Set_StateEnd(false);

	CSoundMgr::Get_Instance()->StopAll();
	CSoundMgr::Get_Instance()->PlayBGM(L"25 Attack.mp3");

	m_pGameObject_Manager = CGameObject_Manager::Get_Instance();

	m_dwStageNumber = CCombat_Manager::Get_Instance()->Get_StageNumber();

	m_tSourceInfo.vPos = { 400.f, 300.f, 0.f };
	m_tSourceInfo.vDir = { 1.f, 1.f, 0.f };
	m_tSourceInfo.vSize = { 4.f, 4.f, 0.f };

	m_tDestInfo.vPos = { 400.f, 300.f, 0.f };
	m_tDestInfo.vDir = { 1.f, 1.f, 0.f };
	m_tDestInfo.vSize = { 4.f, 4.f, 0.f };
	
	m_pAnimClipObjSource = CAnimClip_Manager::Get_Instance()->Get_Clip(m_pObjSource->Get_Name(), L"Attack");

	m_pAnimClipObjDest = CAnimClip_Manager::Get_Instance()->Get_Clip(m_pObjDest->Get_Name(), L"Attack");

	m_bSourceAnimEnd = false;
	m_bDestAnimEnd = false;

	m_fEndTime = 2.f;
	m_fDelay = 2.f;




	return S_OK;
}

void CBattleScene::Update_Scene()
{
	CEffect_Manager::Get_Instance()->Update_Effect_Manager();

	if (CEffect_Manager::Get_Instance()->Get_StateEnd() == false)
		return;
	if (!m_pAnimClipObjSource->bClipEnd && !m_bSourceAnimEnd)
	{
		CAnimClip_Manager::Get_Instance()->ClipFrameMove(m_pAnimClipObjSource, 0.5f);
	}
	else if (m_pAnimClipObjSource->bClipEnd && !m_bSourceAnimEnd)
	{
		m_bSourceAnimEnd = true;
		m_pAnimClipObjSource->bClipEnd = false;
//		m_pObjDest->Get_UnitInfo().iCurHp -= m_pObjSource->Get_UnitInfo().iAtt;
		D3DVECTOR vPos = { 250.f,190.f,0.0f };
		CEffect_Manager::Get_Instance()->AddDigitEffect(m_pObjSource->Get_UnitInfo().iAtt - m_pObjDest->Get_UnitInfo().iDef, vPos);

		if(m_pObjDest->AttackedObject(m_pObjSource))
		{
			if (m_pObjDest->Get_ID() == PLAYER)
			{
				vPos = { 250.f,190.f,0.0f };
				CEffect_Manager::Get_Instance()->Add_Etc_Effect(vPos, 0);
			}
			else
			{
				vPos = { 550.f,190.f,0.0f };
				CEffect_Manager::Get_Instance()->Add_Etc_Effect(vPos, 0);
			}

		}
	}
	if(!m_pAnimClipObjDest->bClipEnd && m_bSourceAnimEnd && !m_bDestAnimEnd)
	{
		if (m_fDelay > 0)
			m_fDelay -= CTime_Manager::Get_Instance()->Get_DeltaTime();

		else CAnimClip_Manager::Get_Instance()->ClipFrameMove(m_pAnimClipObjDest, 0.5f);
	}
	else if (m_pAnimClipObjDest->bClipEnd && !m_bDestAnimEnd)
	{
		m_bDestAnimEnd = true;
		m_pAnimClipObjDest->bClipEnd = false;
//		m_pObjSource->Get_UnitInfo().iCurHp -= m_pObjDest->Get_UnitInfo().iAtt;
		D3DVECTOR vPos = { 550.f,190.f,0.0f };
		CEffect_Manager::Get_Instance()->AddDigitEffect(m_pObjDest->Get_UnitInfo().iAtt - m_pObjSource->Get_UnitInfo().iDef, vPos);

		if (m_pObjSource->AttackedObject(m_pObjDest))
		{
			if (m_pObjSource->Get_ID() == PLAYER)
			{
				vPos = { 550.f,190.f,0.0f };
				CEffect_Manager::Get_Instance()->Add_Etc_Effect(vPos, 0);
			}
			else
			{
				vPos = { 250.f,190.f,0.0f };
				CEffect_Manager::Get_Instance()->Add_Etc_Effect(vPos, 0);
			}

		}
	}
	if (m_bDestAnimEnd && m_bSourceAnimEnd)
	{
		m_fEndTime -= CTime_Manager::Get_Instance()->Get_DeltaTime();
	}
	if (m_fEndTime < 0)
	{
		m_fEndTime = 2.f;
		
		m_fDelay = 2.f;

		CEffect_Manager::Get_Instance()->Clear_Effect_List();

		if(m_pObjSource->Get_ID()==ID::PLAYER)
			CUI_Manager::Get_Instance()->SelectUnit(m_pObjSource);
		else if(m_pObjDest->Get_ID() == ID::PLAYER)
			CUI_Manager::Get_Instance()->SelectUnit(m_pObjDest);

		CScene_Manager::Get_Instance()->Return_Prev_Scene();

	}
//	m_pAnimClipObjSource
	//m_pAnimClipObjDest->FrameMove();
}

void CBattleScene::Render_Scene()
{
	D3DXMATRIX matScale, matTrans, matWorld;

	float fOffsetY = 150.f;

	float fCenterX = 0.f;
	float fCenterY = 0.f;

#pragma region BackGround
	D3DXMatrixScaling(&matScale, 4.0f, 4.0f, 0.f);
	D3DXMatrixTranslation(&matTrans, CScroll_Manager::Get_Instance()->Get_Scroll(CScroll_Manager::eSB_HORZ), CScroll_Manager::Get_Instance()->Get_Scroll(CScroll_Manager::eSB_VERT), 0.f);
	matWorld = matScale * matTrans;
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_Manager(L"Terrain", L"LocalMap", m_dwStageNumber);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);// 배경은 -8.f 만큼 보정해줌
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(-8.f, -8.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


#pragma endregion

#pragma region UnitFace
	const TEXINFO* pFaceInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_Manager(L"Unit", L"Face", m_pObjSource->Get_UnitInfo().dwDrawId);

	fCenterX = pFaceInfo->tImageInfo.Width >> 1;
	fCenterY = pFaceInfo->tImageInfo.Height >> 1;
	D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);
	D3DXMatrixTranslation(&matTrans, DIALOG_OffsetX, DIALOG_OffsetY, 0.f);
	matWorld = matScale * matTrans;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pFaceInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	const TEXINFO* pDestFaceInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_Manager(L"Unit", L"Face", m_pObjDest->Get_UnitInfo().dwDrawId);
	D3DXMatrixScaling(&matScale, -2.f, 2.f, 0.f);
	D3DXMatrixTranslation(&matTrans, WINCX - DIALOG_OffsetX, DIALOG_OffsetY, 0.f);
	matWorld = matScale * matTrans;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

	fCenterX = pDestFaceInfo->tImageInfo.Width >> 1;
	fCenterY = pDestFaceInfo->tImageInfo.Height >> 1;
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pDestFaceInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
#pragma endregion


#pragma region BattleMapUI
	const TEXINFO* pBattleMapUI = CTexture_Manager::Get_Instance()->Get_TexInfo_Manager(L"UI", L"BattleMapUI",0);
	if (nullptr == pBattleMapUI)
		return;

	//D3DXMatrixScaling(&matScale, 1.0f, 1.0f, 0.f);
	D3DXMatrixScaling(&matScale, m_tSourceInfo.vSize.x, m_tSourceInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tSourceInfo.vPos.x, m_tSourceInfo.vPos.y, 0.f);
	matWorld = /*matScale * */matTrans;
	float fUICenterX = pBattleMapUI->tImageInfo.Width >> 1;
	float fUICenterY = pBattleMapUI->tImageInfo.Height >> 1;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pBattleMapUI->pTexture, nullptr, &D3DXVECTOR3(fUICenterX, fUICenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

#pragma endregion


#pragma region SourceObj(Right)
	const TEXINFO* pSourceTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_Manager(m_pAnimClipObjSource->wstrObjectKey, m_pAnimClipObjSource->wstrStateKey, (DWORD)m_pAnimClipObjSource->fFrameStart);
	if (nullptr == pSourceTexInfo)
		return;
	

	D3DXMatrixScaling(&matScale, m_tSourceInfo.vSize.x, m_tSourceInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tSourceInfo.vPos.x, m_tSourceInfo.vPos.y - fOffsetY, 0.f);
	matWorld = matScale * matTrans;
	fCenterX = pSourceTexInfo->tImageInfo.Width >> 1;
	fCenterY = pSourceTexInfo->tImageInfo.Height >> 1;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pSourceTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

#pragma endregion

#pragma region DestObj(Left)
	const TEXINFO* pDestTextInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_Manager(m_pAnimClipObjDest->wstrObjectKey, m_pAnimClipObjDest->wstrStateKey, (DWORD)m_pAnimClipObjDest->fFrameStart);
	if (nullptr == pDestTextInfo)
		return;

	/*if (m_pPlayerAnimClip->wstrStateKey == L"Attack")
	{
		D3DXMatrixScaling(&matScale, -m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	}*/
	//D3DXMatrixScaling(&matScale, m_tDestInfo.vSize.x, m_tDestInfo.vSize.y, 0.f);

	D3DXMatrixScaling(&matScale, -m_tDestInfo.vSize.x, m_tDestInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tDestInfo.vPos.x, m_tDestInfo.vPos.y - fOffsetY, 0.f);
	matWorld = matScale * matTrans;

	fCenterX = pDestTextInfo->tImageInfo.Width >> 1;
	fCenterY = pDestTextInfo->tImageInfo.Height >> 1;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pDestTextInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	
#pragma endregion

#pragma region UI_TEXT
	auto SourceUnitInfo = m_pObjSource->Get_UnitInfo();
	auto DestUnitInfo = m_pObjDest->Get_UnitInfo();

	wstring wstrName, wstrAtt, wstrDef, wstrHp, wstrExp;


	wstrName = m_pObjSource->Get_Name();
	wstrAtt = to_wstring(SourceUnitInfo.iAtt);
	wstrDef = to_wstring(SourceUnitInfo.iDef);
	wstrHp = to_wstring(SourceUnitInfo.iCurHp);
	wstrHp += L"/";
	wstrHp += to_wstring(SourceUnitInfo.iMaxHp);

	wstrExp = to_wstring(SourceUnitInfo.iExp);
	wstrExp += L"/";
	wstrExp += to_wstring(SourceUnitInfo.iMaxExp);
	

	D3DXMatrixTranslation(&matTrans, 660, 50, 0.f);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
	CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), wstrName.c_str(), lstrlen(wstrName.c_str()), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMatrixTranslation(&matTrans, 740, 430, 0.f);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
	CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), wstrAtt.c_str(), lstrlen(wstrAtt.c_str()), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMatrixTranslation(&matTrans, 740, 470, 0.f);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
	CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), wstrDef.c_str(), lstrlen(wstrDef.c_str()), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMatrixTranslation(&matTrans, 730, 550, 0.f);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
	CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), wstrHp.c_str(), lstrlen(wstrHp.c_str()), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (m_pObjSource->Get_ID() == ID::PLAYER)
	{
		D3DXMatrixTranslation(&matTrans, 550, 550, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), wstrExp.c_str(), lstrlen(wstrExp.c_str()), nullptr, 0, D3DCOLOR_ARGB(255, 0, 255, 0));

	}
	///////////////////////////////////////////////////////////////////////////////////

	wstrName = m_pObjDest->Get_Name();
	wstrAtt = to_wstring(DestUnitInfo.iAtt);
	wstrDef = to_wstring(DestUnitInfo.iDef);
	wstrHp = to_wstring(DestUnitInfo.iCurHp);
	wstrHp += L"/";
	wstrHp += to_wstring(DestUnitInfo.iMaxHp);

	wstrExp = to_wstring(DestUnitInfo.iExp);
	wstrExp += L"/";
	wstrExp += to_wstring(DestUnitInfo.iMaxExp);

	D3DXMatrixTranslation(&matTrans, 50, 50, 0.f);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
	CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), wstrName.c_str(), lstrlen(wstrName.c_str()), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMatrixTranslation(&matTrans, 80, 430, 0.f);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
	CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), wstrAtt.c_str(), lstrlen(wstrAtt.c_str()), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMatrixTranslation(&matTrans, 80, 470, 0.f);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
	CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), wstrDef.c_str(), lstrlen(wstrDef.c_str()), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMatrixTranslation(&matTrans, 80, 550, 0.f);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
	CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), wstrHp.c_str(), lstrlen(wstrHp.c_str()), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));


	if (m_pObjDest->Get_ID() == ID::PLAYER)
	{
		D3DXMatrixTranslation(&matTrans, 250, 550, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), wstrExp.c_str(), lstrlen(wstrExp.c_str()), nullptr, 0, D3DCOLOR_ARGB(255, 0, 255, 0));

	}
#pragma endregion
	
	if (CEffect_Manager::Get_Instance()->Get_StateEnd() == false)
	{
		CEffect_Manager::Get_Instance()->FadeOutEffect();
	}
	
	CEffect_Manager::Get_Instance()->Render_Effect_Manager();
}

void CBattleScene::Release_Scene()
{
	CScene_Manager::Get_Instance()->Return_Prev_Scene();
}

CScene* CBattleScene::Create(CGameObject* pObjectSource,CGameObject* pObjectDest)
{
	CBattleScene* pInstance = new CBattleScene;
	pInstance->Set_ObjSource(pObjectSource);
	pInstance->Set_ObjDest(pObjectDest);
	if (FAILED(pInstance->Ready_Scene()))
	{
		delete pInstance;
		return nullptr;
	}

	return pInstance;
}
