#include "stdafx.h"
#include "GameObject.h"
#include "Terrain.h"

CGameObject::CGameObject()
	:m_bIsMove(false)
	,m_bIsSelected(false)
	,m_bIsAttack(false)
	,m_bTurn(false)
	,m_bIsDead(false)
	, m_bIsMoving(false)
	, m_dwUnitState(0)
	, m_pTarget(nullptr)
	, m_dwLocate(0)
	, m_PosMoveList({})
	, m_tRect({})
	, m_eId(END)
{
}


CGameObject::~CGameObject()
{
}

void CGameObject::Set_Locate(DWORD dwLocate)
{
	m_dwLocate = dwLocate;

	float PosX = (dwLocate % TILEX) * TILECX;
	float PosY = (dwLocate / TILEX) * TILECY;

	this->m_tInfo.vPos = { PosX, PosY, 0.f };
	this->m_tInfo.vDir = { 1.f, 1.f, 0.f };
	this->m_tInfo.vSize = { 3.0f,3.0f, 0.f };
}

void CGameObject::Set_DESC(UNIT_DESC* pDESC)
{
	m_wstrName = pDESC->wstr_name;
	m_tInfo = pDESC->tInfo;
	m_tUnitInfo = pDESC->tUnitInfo;
	m_dwLocate = pDESC->dwLocate;
}

void CGameObject::Set_Hp(int HpPoint)
{
	m_tUnitInfo.iCurHp += HpPoint;
	if (m_tUnitInfo.iCurHp > m_tUnitInfo.iMaxHp)
		m_tUnitInfo.iCurHp = m_tUnitInfo.iMaxHp;
}

void CGameObject::FrameMove(float fSpeed)
{
	/*m_tCurAnimClip.fFrameStart += m_tCurAnimClip.fFrameEnd * CTime_Manager::Get_Instance()->Get_DeltaTime() * fSpeed;
	if (m_tCurAnimClip.fFrameEnd < m_tCurAnimClip.fFrameStart)
		m_tCurAnimClip.fFrameStart = 0.f;*/

	m_pCurAnimClip->fFrameStart += m_pCurAnimClip->fFrameEnd * CTime_Manager::Get_Instance()->Get_DeltaTime() * fSpeed;
	if (m_pCurAnimClip->fFrameEnd < m_pCurAnimClip->fFrameStart)
		m_pCurAnimClip->fFrameStart = 0.f;
}

void CGameObject::Change_AnimState(wstring objectKey,wstring stateKey)
{
	m_pNextAnimClip = CAnimClip_Manager::Get_Instance()->Get_Clip(objectKey, stateKey);
	m_pCurAnimClip = m_pNextAnimClip;
}

int CGameObject::AttackedObject(CGameObject* pObject)
{
	auto iAtkObj = pObject->Get_UnitInfo().iAtt;

	if (iAtkObj <= this->m_tUnitInfo.iDef)
		return 0;

	this->m_tUnitInfo.iCurHp -= iAtkObj - m_tUnitInfo.iDef;

	if (this->m_tUnitInfo.iCurHp < 0)
	{
		this->m_tUnitInfo.iCurHp = 0;
		if (pObject->Get_ID() == ID::PLAYER)
		{
			if(pObject->Add_Exp(m_tUnitInfo.iMaxHp))
			{
				return 1;
			}
			else
				return 0;
		}
	}

	if (pObject->Get_ID() == ID::PLAYER)
	{
		if (pObject->Add_Exp(iAtkObj - this->m_tUnitInfo.iDef))
			return 1;
		else
			return 0;
	}
	return 0;
}

void CGameObject::SetMovePosGameObject(int movePosIndex)
{
	CTerrain* pTerrain = dynamic_cast<CTerrain*>(CGameObject_Manager::Get_Instance()->Get_Terrain(0));
	auto& vecTile = pTerrain->Get_VecTile();

	m_PosMoveList.emplace_back(vecTile[movePosIndex]);
}

void CGameObject::PosFrameMove()
{
	if (m_PosMoveList.empty())
	{
		if(m_pCurAnimClip->wstrStateKey!=L"Idle")
			Change_AnimState(m_wstrName, L"Idle");
		//if (CCombat_Manager::Get_Instance()->Get_SelectedObject() != dynamic_cast<CGameObject*>(this))
		//{
		//	if (m_bIsMoving)
		//	{
		//		CKey_Manager::Get_Instance()->Set_KeyLock(false);
		//		//m_bIsMoving = false;
		//	}
		//}
		return;
	}
	m_tInfo.vDir = m_PosMoveList.front()->vPos - m_tInfo.vPos;
	float fDist = D3DXVec3Length(&m_tInfo.vDir);
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
	//m_bIsMoving = true;

	if (3.f > fDist)
	{
		//m_dwUnitState |= UNIT_MOVE;
		m_dwLocate = m_PosMoveList.front()->iIndex;
		m_tInfo.vPos = m_PosMoveList.front()->vPos;
		m_tUnitInfo.iCurMoveRange -= 1;
		//	printf("%f %f %f\n", m_tInfo.vPos.x, m_tInfo.vPos.y, m_tInfo.vPos.z);
		m_PosMoveList.pop_front();
	}
	if (m_tInfo.vDir.x < -0.9)
		Change_AnimState(m_wstrName, L"Move_Left");

	if (m_tInfo.vDir.x > 0.9)
		Change_AnimState(m_wstrName, L"Move_Right");

	if (m_tInfo.vDir.y < -0.9)
		Change_AnimState(m_wstrName, L"Move_Back");

	if (m_tInfo.vDir.y > 0.9)
		Change_AnimState(m_wstrName, L"Move_Front");

	m_tInfo.vPos += m_tInfo.vDir * CTime_Manager::Get_Instance()->Get_DeltaTime() * m_fSpeed;
	CSoundMgr::Get_Instance()->PlaySound(L"Walk0.wav", CSoundMgr::PLAYER);
}

int CGameObject::Add_Exp(int iExp)
{
	m_tUnitInfo.iExp += iExp;
	while (m_tUnitInfo.iExp >= m_tUnitInfo.iMaxExp)
	{
		m_tUnitInfo.iLevel++;
	
		m_tUnitInfo.iExp -= m_tUnitInfo.iMaxExp;
		m_tUnitInfo.iMaxExp=static_cast<int>(m_tUnitInfo.iMaxExp*3.f);

		m_tUnitInfo.iMaxHp = static_cast<int>(m_tUnitInfo.iMaxHp * 1.1f);
		m_tUnitInfo.iCurHp = m_tUnitInfo.iMaxHp;
		m_tUnitInfo.iAtt = static_cast<int>(m_tUnitInfo.iAtt * 1.2f);
		m_tUnitInfo.iDef++;
		
		return 1;
	}
	return 0;
}

