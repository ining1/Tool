#include "stdafx.h"
#include "..\Public\MainApp.h"
#include "Level_Logo.h"
#include "Level_Loading.h"
#ifdef _DEBUG
#include "Console_Debug.h"
#include "ImGUI_Manager.h"
#endif // _DEBUG


CMainApp::CMainApp()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::NativeConstruct()
{
	CGraphic_Device::GRAPHICDESC GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof(CGraphic_Device::GRAPHICDESC));

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.iWinCX = g_iWinCX;
	GraphicDesc.iWinCY = g_iWinCY;
	GraphicDesc.eWinMode = CGraphic_Device::MODE_WIN;

	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst, LEVEL_END, GraphicDesc, &m_pDevice, &m_pDeviceContext)))
	{
		MSG_BOX(L"Failed To CMainApp : NativeConstruct : m_pGameInstance->Initialize_Engine");
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		MSG_BOX(L"Failed To CMainApp : NativeConstruct : SetUp_Components");
		return E_FAIL;
	}

	if (FAILED(Open_Level(LEVEL_TEST)))
	{
		MSG_BOX(L"Failed To CMainApp : NativeConstruct : Open_Level");
		return E_FAIL;
	}
#ifndef _DEBUG
	if (FAILED(m_pGameInstance->Init_Sound(L"../Resource/Sound/")))
	{
		MSG_BOX(L"Failed To CMainApp : NativeConstruct : m_pGameInstance->Init_Sound");
		return E_FAIL;
	}
#endif // !_DEBUG

#ifdef _DEBUG
	//if (FAILED(Init_Console_Debug()))
	//{
	//	MSG_BOX(L"Failed To CMainApp : NativeConstruct : Init_Console_Debug");
	//	return E_FAIL;
	//}
	if (FAILED(Init_Imgui()))
	{
		MSG_BOX(L"Failed To CMainApp : NativeConstruct : Init_Imgui");
		return E_FAIL;
	}
#endif // _DEBUG

	return S_OK;
}

void CMainApp::Tick(_double TimeDelta)
{
#ifdef _DEBUG
	//CConsole_Debug* pConsole_Debug = GET_INSTANCE(CConsole_Debug);

//	pConsole_Debug->Tick(TimeDelta);

	//RELEASE_INSTANCE(CConsole_Debug);
#endif // _DEBUG

	if (nullptr == m_pGameInstance)
	{
		MSG_BOX(L"Failed To CMainApp : Tick : nullptr == m_pGameInstance");
		return;
	}
	Key_Input();
	
	m_pGameInstance->Tick_Engine(TimeDelta);

}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance)
	{
		MSG_BOX(L"Failed To CMainApp : Render : nullptr == m_pGameInstance");
		return E_FAIL;
	}
	if (nullptr == m_pRenderer)
	{
		MSG_BOX(L"Failed To CMainApp : Render : nullptr == m_pRenderer");
		return E_FAIL;
	}

	m_pGameInstance->Render_Begin(_float4(0.5f, 0.5f, 0.5f, 1.f));

	if (true == g_bdebug)
	{
		m_pRenderer->Render_GameObjects();
		m_pGameInstance->Render_Engine();
	}

#ifdef _DEBUG
	CImGUI_Manager* pImgui = GET_INSTANCE(CImGUI_Manager);
	pImgui->Render();
	RELEASE_INSTANCE(CImGUI_Manager);
#endif //_DEBUG
	m_pGameInstance->Render_End();
	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVEL eLevel)
{
	if (nullptr == m_pGameInstance)
	{
		MSG_BOX(L"Failed To CMainApp : Open_Level : nullptr == m_pGameInstance");
		return E_FAIL;
	}
	CLevel* pLevel = nullptr;

	LEVEL eLevel_Current = LEVEL_END;
	switch (eLevel)
	{
	case LEVEL_LOGO:
		eLevel_Current = LEVEL_LOGO;
		pLevel = CLevel_Logo::Create(m_pDevice, m_pDeviceContext);
		break;
	default:
		eLevel_Current = LEVEL_LOADING;
		pLevel = CLevel_Loading::Create(m_pDevice, m_pDeviceContext, eLevel);
		break;
	}

	if (FAILED(m_pGameInstance->Open_Level(eLevel_Current, pLevel)))
	{
		MSG_BOX(L"Failed To CMainApp : Open_Level : m_pGameInstance->Open_Level");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CMainApp::SetUp_Components()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Renderer", m_pRenderer = CRenderer::Create(m_pDevice, m_pDeviceContext))))
	{
		MSG_BOX(L"Failed To CMainApp : SetUp_Components : m_pGameInstance->Add_Prototype(Renderer)");
		return E_FAIL;
	}
	Safe_AddRef(m_pRenderer);

	return S_OK;
}

void CMainApp::Key_Input()
{
	if (nullptr == m_pGameInstance)
	{
		MSG_BOX(L"Failed To CMainApp : Key_Input : nullptr == m_pGameInstance");
		return;
	}
	if (m_pGameInstance->Key_Down(DIK_RETURN))
	{
		if (false == m_bPause)
		{
			m_bPause = true;
		}
		else
		{
			m_bPause = false;
		}
	}
}

#ifdef _DEBUG
HRESULT CMainApp::Init_Imgui()
{
	CImGUI_Manager* pImgui = GET_INSTANCE(CImGUI_Manager);

	pImgui->InitWin32(g_hWnd);
	pImgui->Init_ImGui(&m_pDevice, &m_pDeviceContext);

	RELEASE_INSTANCE(CImGUI_Manager);
	return S_OK;
}

HRESULT CMainApp::Init_Console_Debug()
{
	CConsole_Debug* pConsole_Debug = GET_INSTANCE(CConsole_Debug);

	pConsole_Debug->NativeConstruct();

	RELEASE_INSTANCE(CConsole_Debug);
	return S_OK;
}
#endif //Debug

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp();
	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX(L"Failed To CMainApp : Create : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pRenderer);
	m_pGameInstance->Release_CurrentLevel();
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);

#ifdef _DEBUG
	//CConsole_Debug::GetInstance()->DestroyInstance();
#endif // _DEBUG

	CGameInstance::Release_Engine();
}