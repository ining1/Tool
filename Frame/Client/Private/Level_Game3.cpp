#include "stdafx.h"
#include "..\Public\Level_Game3.h"
#include "Level_Loading.h"

CLevel_Game3::CLevel_Game3(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CLevel(pDevice, pDeviceContext)
{
}

HRESULT CLevel_Game3::NativeConstruct()
{
	return S_OK;
}

void CLevel_Game3::Tick(_double TimeDelta)
{
	if (m_pGameInstance->Key_Down(DIK_SPACE))
	{
		m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_TEST));
	}
}

HRESULT CLevel_Game3::Render()
{
	SetWindowText(g_hWnd, L"Game3");
	return S_OK;
}

CLevel_Game3* CLevel_Game3::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CLevel_Game3* pInstance = new CLevel_Game3(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX(L"Failed To CLevel_Game3 : Create : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_Game3::Free()
{
	__super::Free();
}