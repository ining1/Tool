#include "stdafx.h"
#include "..\Public\Level_Game2.h"
#include "Level_Loading.h"

CLevel_Game2::CLevel_Game2(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CLevel(pDevice, pDeviceContext)
{
}

HRESULT CLevel_Game2::NativeConstruct()
{
	return S_OK;
}

void CLevel_Game2::Tick(_double TimeDelta)
{
	if (m_pGameInstance->Key_Down(DIK_SPACE))
	{
		m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_GAME3));
	}
}

HRESULT CLevel_Game2::Render()
{
	SetWindowText(g_hWnd, L"Game2");
	return S_OK;
}

CLevel_Game2* CLevel_Game2::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CLevel_Game2* pInstance = new CLevel_Game2(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX(L"Failed To CLevel_Game2 : Create : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_Game2::Free()
{
	__super::Free();
}