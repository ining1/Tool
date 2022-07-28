#include "stdafx.h"
#include "..\Public\Level_Loby.h"
#include "Level_Loading.h"

CLevel_Loby::CLevel_Loby(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CLevel(pDevice, pDeviceContext)
{
}

HRESULT CLevel_Loby::NativeConstruct()
{
	return S_OK;
}

void CLevel_Loby::Tick(_double TimeDelta)
{
	if (m_pGameInstance->Key_Down(DIK_SPACE))
	{
		m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_TUTORIAL));
	}
}

HRESULT CLevel_Loby::Render()
{
	SetWindowText(g_hWnd, L"Loby");
	return S_OK;
}

CLevel_Loby* CLevel_Loby::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CLevel_Loby* pInstance = new CLevel_Loby(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX(L"Failed To CLevel_Loby : Create : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_Loby::Free()
{
	__super::Free();
}