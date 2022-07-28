#include "stdafx.h"
#include "..\Public\Level_Test.h"
#include "Level_Loading.h"
#include "Camera_Default.h"

CLevel_Test::CLevel_Test(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CLevel(pDevice, pDeviceContext)
{
}

HRESULT CLevel_Test::NativeConstruct()
{
	if (FAILED(Ready_ForCamera()))
	{
		MSG_BOX(L"Failed To CLevel_Test : NativeConstruct : Ready_ForCamera");
		return E_FAIL;
	}
	if (FAILED(Ready_ForTest()))
	{
		MSG_BOX(L"Failed To CLevel_Test : NativeConstruct : Ready_ForTest");
		return E_FAIL;
	}
	return S_OK;
}

void CLevel_Test::Tick(_double TimeDelta)
{
}

HRESULT CLevel_Test::Render()
{
	SetWindowText(g_hWnd, L"Test");
	return S_OK;
}

HRESULT CLevel_Test::Ready_ForCamera()
{
	CCamera::CAMERADESC CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));
	CameraDesc.vEye = _float3(0.f, 0.f, -5.f);
	CameraDesc.vAt = _float3(0.f, 0.1f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 200.f;
	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;

	if (nullptr == m_pGameInstance->Add_GameObjectToLayer(LEVEL_TEST, L"Layer_Camera", L"Prototype_GameObject_Camera_Default", &CameraDesc))
	{
		MSG_BOX(L"Failed To CLevel_Test : Ready_ForCamera : m_pGameInstance->Add_GameObjectToLayer(Camera_Default)");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CLevel_Test::Ready_ForTest()
{
	/*if (nullptr == m_pGameInstance->Add_GameObjectToLayer(LEVEL_TEST, L"Layer_Test", L"Prototype_GameObject_Test"))
	{
		MSG_BOX(L"Failed To CLevel_Test : Ready_ForTest : m_pGameInstance->Add_GameObjectToLayer(Test)");
		return E_FAIL;
	}*/

	if (nullptr == m_pGameInstance->Add_GameObjectToLayer(LEVEL_TEST, L"Layer_Test", L"Prototype_GameObject_Rect_Effect"))
	{
	MSG_BOX(L"Failed To CLevel_Test : Ready_ForTest : m_pGameInstance->Add_GameObjectToLayer(Test)");
	return E_FAIL;
	}
	return S_OK;
}

CLevel_Test* CLevel_Test::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CLevel_Test* pInstance = new CLevel_Test(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX(L"Failed To CLevel_Test : Create : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_Test::Free()
{
	__super::Free();
}