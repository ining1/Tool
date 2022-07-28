#include "stdafx.h"
#include "..\Public\Level_Tutorial.h"
#include "Level_Loading.h"

CLevel_Tutorial::CLevel_Tutorial(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CLevel(pDevice, pDeviceContext)
{
}

HRESULT CLevel_Tutorial::NativeConstruct()
{
	return S_OK;
}

void CLevel_Tutorial::Tick(_double TimeDelta)
{
	if (m_pGameInstance->Key_Down(DIK_SPACE))
	{
		m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_GAME1));
	}
}

HRESULT CLevel_Tutorial::Render()
{
	SetWindowText(g_hWnd, L"Tutorial");
	return S_OK;
}

CLevel_Tutorial* CLevel_Tutorial::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CLevel_Tutorial* pInstance = new CLevel_Tutorial(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX(L"Failed To CLevel_Tutorial : Create : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_Tutorial::Free()
{
	__super::Free();
}