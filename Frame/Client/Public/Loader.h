#pragma once
#include "Client_Define.h"
#include "Base.h"
#include "GameInstance.h"

BEGIN(Client)

class CLoader final : public CBase
{
private:
	explicit CLoader(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual ~CLoader() = default;
public:
	HRESULT NativeConstruct(LEVEL eLevel);
public:
	CRITICAL_SECTION Get_CriticalSection();
	LEVEL Get_LevelIndex();
	_bool Get_Finished() const;
	const _tchar* Get_LoadingText() const;
public:
	HRESULT Ready_ForLobyLevel();
	HRESULT Ready_ForTutorialLevel();
	HRESULT Ready_ForGame1Level();
	HRESULT Ready_ForGame2Level();
	HRESULT Ready_ForGame3Level();
	HRESULT Ready_ForTestLevel();
private:
	LEVEL m_eLevel = LEVEL_END;
	_bool m_bFinished = false;
	_tchar m_szLoading[MAX_PATH] = L"";
private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;
	HANDLE m_hThread = 0;
	CRITICAL_SECTION m_CriticalSection;
private:
	CGameInstance* m_pGameInstance = nullptr;
public:
	static CLoader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, LEVEL eLevel);
	virtual void Free() override;
};

END