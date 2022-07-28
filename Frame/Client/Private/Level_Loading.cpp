#include "stdafx.h"
#include "..\Public\Level_Loading.h"
#include "Level_Loby.h"
#include "Level_Loby.h"
#include "Level_Tutorial.h"
#include "Level_Game1.h"
#include "Level_Game2.h"
#include "Level_Game3.h"
#include "Level_Test.h"

CLevel_Loading::CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CLevel(pDevice, pDeviceContext)
{
}

HRESULT CLevel_Loading::NativeConstruct(LEVEL eNextLevel)
{
	m_eNextLevel = eNextLevel;
	m_pLoader = CLoader::Create(m_pDevice, m_pDeviceContext, eNextLevel);
	if (nullptr == m_pLoader)
	{
		MSG_BOX(L"CLevel_Loading -> NativeConstruct -> nullptr == m_pLoader");
		return E_FAIL;
	}
	return S_OK;
}

void CLevel_Loading::Tick(_double TimeDelta)
{
	if (true == m_pLoader->Get_Finished())
	{
		CLevel* pLevel = nullptr;
		switch (m_eNextLevel)
		{
		case LEVEL_LOBY:
			pLevel = CLevel_Loby::Create(m_pDevice, m_pDeviceContext);
			break;
		case LEVEL_TUTORIAL:
			pLevel = CLevel_Tutorial::Create(m_pDevice, m_pDeviceContext);
			break;
		case LEVEL_GAME1:
			pLevel = CLevel_Game1::Create(m_pDevice, m_pDeviceContext);
			break;
		case LEVEL_GAME2:
			pLevel = CLevel_Game2::Create(m_pDevice, m_pDeviceContext);
			break;
		case LEVEL_GAME3:
			pLevel = CLevel_Game3::Create(m_pDevice, m_pDeviceContext);
			break;
		case LEVEL_TEST:
			pLevel = CLevel_Test::Create(m_pDevice, m_pDeviceContext);
			break;
		}
		if (FAILED(m_pGameInstance->Open_Level(m_eNextLevel, pLevel)))
		{
			MSG_BOX(L"CLevel_Loading -> Tick -> m_pGameInstance->Open_Level");
			return;
		}
	}
}

HRESULT CLevel_Loading::Render()
{
	_tchar szText[MAX_PATH] = L"";

	lstrcpy(szText, L"Loading : ");
	lstrcat(szText, m_pLoader->Get_LoadingText());

	SetWindowText(g_hWnd, szText);
	return S_OK;
}

CLevel_Loading* CLevel_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, LEVEL eNextLevel)
{
	CLevel_Loading*	pInstance = new CLevel_Loading(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(eNextLevel)))
	{
		MSG_BOX(L"CLevel_Loading -> Create -> pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();
	Safe_Release(m_pLoader);
}