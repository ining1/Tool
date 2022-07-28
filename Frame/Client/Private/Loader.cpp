#include "stdafx.h"
#include "..\Public\Loader.h"
#include "Camera_Default.h"
#include "Test_Model.h"
#include "Rect_Effect.h"
#include "Point_Effect.h"


CLoader::CLoader(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	:m_pDevice(pDeviceOut)
	,m_pDeviceContext(pDeviceContextOut)
	,m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pDeviceContext);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pGameInstance);
}

unsigned int APIENTRY ThreadEntryFunc(void* pArg)
{
	CLoader* pLoader = (CLoader*)pArg;
	EnterCriticalSection(&pLoader->Get_CriticalSection());
	switch (pLoader->Get_LevelIndex())
	{
	case LEVEL_LOBY:
		pLoader->Ready_ForLobyLevel();
		break;
	case LEVEL_TUTORIAL:
		pLoader->Ready_ForTutorialLevel();
		break;
	case LEVEL_GAME1:
		pLoader->Ready_ForGame1Level();
		break;
	case LEVEL_GAME2:
		pLoader->Ready_ForGame2Level();
		break;
	case LEVEL_GAME3:
		pLoader->Ready_ForGame3Level();
		break;
	case LEVEL_TEST:
		pLoader->Ready_ForTestLevel();
		break;
	}
	LeaveCriticalSection(&pLoader->Get_CriticalSection());
	return 0;
}

HRESULT CLoader::NativeConstruct(LEVEL eLevel)
{
	m_eLevel = eLevel;
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	InitializeCriticalSection(&m_CriticalSection);
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadEntryFunc, this, 0, nullptr);
	if (0 == m_hThread)
	{
		MSG_BOX(L"Failed To CLoader : NativeConstruct : 0 == m_hThread");
		return E_FAIL;
	}
	return S_OK;
}

CRITICAL_SECTION CLoader::Get_CriticalSection()
{
	return m_CriticalSection;
}

LEVEL CLoader::Get_LevelIndex()
{
	return m_eLevel;
}

_bool CLoader::Get_Finished() const
{
	return m_bFinished;
}

const _tchar* CLoader::Get_LoadingText() const
{
	return m_szLoading;
}

HRESULT CLoader::Ready_ForLobyLevel()
{
	lstrcpy(m_szLoading, L"Finish!");
	m_bFinished = true;

	return S_OK;
}

HRESULT CLoader::Ready_ForTutorialLevel()
{
	//For Shader
	lstrcpy(m_szLoading, L"Shader");

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Shader_VtxAnim", CShader::Create(m_pDevice, m_pDeviceContext, L"../ShaderFiles/Shader_VtxAnim.hlsl", VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements))))
	{
		MSG_BOX(L"Failed To CLoader : Ready_ForTutorialLevel : m_pGameInstance->Add_Prototype(Shader_VtxAnim)");
		return E_FAIL;
	}

	

	lstrcpy(m_szLoading, L"Finish!");
	m_bFinished = true;

	return S_OK;
}

HRESULT CLoader::Ready_ForGame1Level()
{
	lstrcpy(m_szLoading, L"Finish!");
	m_bFinished = true;

	return S_OK;
}

HRESULT CLoader::Ready_ForGame2Level()
{
	lstrcpy(m_szLoading, L"Finish!");
	m_bFinished = true;

	return S_OK;
}

HRESULT CLoader::Ready_ForGame3Level()
{
	lstrcpy(m_szLoading, L"Finish!");
	m_bFinished = true;

	return S_OK;
}

HRESULT CLoader::Ready_ForTestLevel()
{
	//For Shader
	lstrcpy(m_szLoading, L"Shader");

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Shader_VtxAnim", CShader::Create(m_pDevice, m_pDeviceContext, L"../ShaderFiles/Shader_VtxAnim.hlsl", VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements))))
	{
		MSG_BOX(L"Failed To CLoader : Ready_ForTestLevel : m_pGameInstance->Add_Prototype(Shader_VtxAnim)");
		return E_FAIL;
	}

	//For Camera
	lstrcpy(m_szLoading, L"Camera");

	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_GameObject_Camera_Default", CCamera_Default::Create(m_pDevice, m_pDeviceContext))))
	{
		MSG_BOX(L"Failed To CLoader : Ready_ForTestLevel : m_pGameInstance->Add_Prototype(Camera_Default)");
		return E_FAIL;
	}

	////For Test
	//lstrcpy(m_szLoading, L"Test");

	//if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_GameObject_Test", CTest_Model::Create(m_pDevice, m_pDeviceContext))))
	//{
	//	MSG_BOX(L"CLoader -> Ready_ForTestLevel -> m_pGameInstance->Add_Prototype(Test)");
	//	return E_FAIL;
	//}

	////For Model
	//lstrcpy(m_szLoading, L"Model");

	//_matrix PivotMatrix;
	//PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TEST, L"Prototype_Component_Model_Test", CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Resource/Model/Test/", "Test.fbx", PivotMatrix))))
	//{
	//	MSG_BOX(L"CLoader -> Ready_ForTestLevel -> m_pGameInstance->Add_Prototype(Model_Test)");
	//	return E_FAIL;
	//}
	
	//////////////////////////////////
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxInstance"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../ShaderFiles/Shader_VtxInstance.hlsl"), VTXINSTANCE_DECLARATION::Elements, VTXINSTANCE_DECLARATION::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Resource/Textures/Effect/Effect(%d).png"), 29))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_RectInstance100"),
		CVIBuffer_Rect_Instance::Create(m_pDevice, m_pDeviceContext, 2000))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance"),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pDeviceContext, 50))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Rect_Effect"),
		CRect_Effect::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Point_Effect"),
		CPoint_Effect::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	///////////////////

	lstrcpy(m_szLoading, L"Finish!");
	m_bFinished = true;

	return S_OK;
}

CLoader* CLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, LEVEL eLevel)
{
	CLoader* pInstance = new CLoader(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(eLevel)))
	{
		MSG_BOX(L"Failed To CLoader : Create : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	DeleteCriticalSection(&m_CriticalSection);
	DeleteObject(m_hThread);
	CloseHandle(m_hThread);

	Safe_Release(m_pGameInstance);

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}