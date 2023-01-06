#include "stdafx.h"
#include "Effect_Digit.h"

void CEffect_Digit::Set_Number(int number)
{
    while (true)
    {
        int lastNum = number % 10;
 
        m_vDigit.push_back(lastNum);
       
        number /= 10;
        if (number == 0)
            break;

    }
}

HRESULT CEffect_Digit::Ready_GameObject()
{
    for (int i = 0; i < 10; i++)
    {
        const TEXINFO* pTextInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_Manager(L"UI", L"digit", i);
        
        m_vDigitTexture.push_back(pTextInfo);
    }
    m_vPos = { 0.f,0.f,0.f };
    m_iAlpha = 255;
    m_RGB = { 255,255,255 };

    return S_OK;
}

int CEffect_Digit::Update_GameObject()
{
    m_iAlpha -= CTime_Manager::Get_Instance()->Get_DeltaTime();
    m_vPos.y -= CTime_Manager::Get_Instance()->Get_DeltaTime()*10.f;

    if (m_bIsDead)
        return OBJ_DEAD;

    return OBJ_NOEVENT;
}

void CEffect_Digit::Late_Update_GameObject()
{
}

void CEffect_Digit::Render_GameObject()
{
    int iSize = m_vDigit.size();
    
    D3DXMATRIX matTrans, matScale, matWorld;
    float fCenterX = 0.f;
    float fCenterY = 0.f;
    
    if (iSize != 0)
    {
        if (m_iAlpha <= 0)
        {
            m_bIsDead = true;
        }
        for (int i = 0; i < iSize; i++)
        {
            //const TEXINFO* pDialogInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_Manager(L"Dialog", L"DialogImg", 0);
            auto curTextInfo = m_vDigitTexture[m_vDigit[i]];
          
            fCenterX = curTextInfo->tImageInfo.Width >> 1;
            fCenterY = curTextInfo->tImageInfo.Height >> 1;
            D3DXMatrixTranslation(&matTrans, -(fCenterX * i*2.1f) + m_vPos.x, fCenterY + m_vPos.y, 0.f);

            matWorld = matTrans;

            CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
            CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(curTextInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, m_RGB.rgbtRed, m_RGB.rgbtGreen, m_RGB.rgbtBlue));
        }
    }
}

void CEffect_Digit::Release_GameObject()
{
}

CGameObject* CEffect_Digit::Create()
{
    CGameObject* pInstance = new CEffect_Digit;
    if (FAILED(pInstance->Ready_GameObject()))
    {
        delete pInstance;
        return nullptr;
    }
    return pInstance;
}
