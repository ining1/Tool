#include "..\Public\Level_Manager.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CLevel_Manager)

CLevel_Manager::CLevel_Manager()
{
}

HRESULT CLevel_Manager::OpenLevel(_uint iLevelIndex, CLevel* pLevel)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (nullptr != m_pCurrentLevel)
	{
		if (FAILED(pGameInstance->Clear_LevelResource(m_iLevelIndex)))
		{
			MSG_BOX(L"Failed To CLevel_Manager : OpenLevel : pGameInstance->Clear_LevelResource");
			return E_FAIL;
		}
	}

	Safe_Release(m_pCurrentLevel);
	m_pCurrentLevel = pLevel;
	m_iLevelIndex = iLevelIndex;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CLevel_Manager::Tick(_double TimeDelta)
{
	if (nullptr == m_pCurrentLevel)
	{
		return;
	}
	m_pCurrentLevel->Tick(TimeDelta);
}

HRESULT CLevel_Manager::Render()
{
	if (nullptr == m_pCurrentLevel)
	{
		MSG_BOX(L"Failed To CLevel_Manager : Render : nullptr == m_pCurrentLevel");
		return E_FAIL;
	}
	return m_pCurrentLevel->Render();
}

void CLevel_Manager::Free()
{
	Safe_Release(m_pCurrentLevel);
}