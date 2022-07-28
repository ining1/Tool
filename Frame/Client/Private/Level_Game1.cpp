#include "stdafx.h"
#include "..\Public\Level_Game1.h"
#include "Level_Loading.h"

CLevel_Game1::CLevel_Game1(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CLevel(pDevice, pDeviceContext)
{
}

HRESULT CLevel_Game1::NativeConstruct()
{
	return S_OK;
}

void CLevel_Game1::Tick(_double TimeDelta)
{
	if (m_pGameInstance->Key_Down(DIK_SPACE))
	{
		m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_GAME2));
	}
}

HRESULT CLevel_Game1::Render()
{
	SetWindowText(g_hWnd, L"Game1");
	return S_OK;
}

CLevel_Game1* CLevel_Game1::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CLevel_Game1* pInstance = new CLevel_Game1(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX(L"Failed To CLevel_Game1 : Create : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_Game1::Free()
{
	__super::Free();
}