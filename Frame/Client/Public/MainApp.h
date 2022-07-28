#pragma once
#include "Client_Define.h"
#include "Base.h"
#include "GameInstance.h"

BEGIN(Client)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;
public:
	HRESULT NativeConstruct();
	void Tick(_double TimeDelta);
	HRESULT Render();
private:
	HRESULT Open_Level(LEVEL eLevel);
	HRESULT SetUp_Components();
private:
	void Key_Input();
#ifdef _DEBUG
private:
	HRESULT Init_Imgui();
	HRESULT Init_Console_Debug();
#endif // _DEBUG
private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;
private:
	CGameInstance* m_pGameInstance = nullptr;
	CRenderer* m_pRenderer = nullptr;
private:
	_bool m_bPause = false;
public:
	static CMainApp* Create();
	virtual void Free() override;
};

END