#include "stdafx.h"
#include "Graphic_Device.h"

IMPLEMENT_SINGLETON(CGraphic_Device)
CGraphic_Device::CGraphic_Device()
{
}


CGraphic_Device::~CGraphic_Device()
{
	Release_Graphic_Device(); 
}

HRESULT CGraphic_Device::Ready_Graphic_Device(MODE eMode)
{
	D3DCAPS9 d3dCaps{};
	//ZeroMemory(&d3dCaps, sizeof(D3DCAPS9)); 

	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION); 
	// 어뎁터 - 현재 진단도구에 나열된 그래픽 카드를 조사하겠다라는 뜻. 
	// 디바이스 타입 - 애는 장치의 정보를 어디서 얻어올 것이냐. 
	// HAL 이라는 곳에서 정보를 얻어오겠다. 
	// 이건 그래픽 카드 사들에서 공통적인 데이터를 여기에 넣어주고 
	// 마소에들이 거기서 데이터 가져다 쓰는 거. 
	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps)))
		goto ERR;

	// 버텍스 프로세싱 - 정점 연산과 조명연산 합친거 
	DWORD vp = 0; 
	
	if (d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	// 정점 변환과 조명연산을 하드웨어에서 처리 하겠다. 
	else 
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	// 정점 변환과 조명연산을 소프트웨어에서 처리 하겠다. 

	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory(&d3dpp, sizeof(d3dpp)); 
	d3dpp.BackBufferWidth = WINCX;
	d3dpp.BackBufferHeight = WINCY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality =0;

	//D3DSWAPEFFECT_DISCARD - 스왑체인 방식을 사용하겠다 라는 뜻. 
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = g_hWND;
	d3dpp.Windowed = eMode;// TRUE 일 경우 창모드, FALSE일 경우 전체화면 모드 
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	/* FullScreen_RefreshRateInHz must be zero for Windowed mode */
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// 장치를 제어하기 위한 컴객체 생성한적 없는데 나는. .. 저건 기본 데이터만 세팅한거고 
	// 이제 여기서 만들어 줘야지 저 정보들을 토대로. 
	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWND, vp, &d3dpp, &m_pDevice)))
		goto ERR; 

	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
		goto ERR; 

	D3DXFONT_DESCW tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW)); 
	tFontInfo.Height = 20; // 높이
	tFontInfo.Width = 10; // 너비
	tFontInfo.Weight = FW_HEAVY;//두께 그럼 홍두께? ㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋ
	tFontInfo.CharSet = HANGUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"배달의민족 주아"); 
	if (FAILED(D3DXCreateFontIndirectW(m_pDevice, &tFontInfo, &m_pFont)))
		goto ERR; 
	if (FAILED(D3DXCreateLine(m_pDevice, &m_pLine)))
		goto ERR; 
	return S_OK;
ERR:
	ERR_MSG(L"Graphic_Device Ready Failed");

	return E_FAIL;
}

void CGraphic_Device::Release_Graphic_Device()
{
	if (m_pLine)
		m_pLine->Release();
	if (m_pFont)
		m_pFont->Release();

	if (m_pSprite)
		m_pSprite->Release();

	if (m_pDevice)
		m_pDevice->Release(); 
	// 
	if (m_pSDK)
		m_pSDK->Release();
}

void CGraphic_Device::Render_Begin()
{
	m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 255, 0), 0.f, 1);
	m_pDevice->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CGraphic_Device::Render_End(HWND hWnd/* = nullptr*/)
{
	m_pSprite->End();
	m_pDevice->EndScene();
	m_pDevice->Present(nullptr, nullptr, hWnd, nullptr);
}
