#include "..\Public\GameInstance.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	:m_pGraphic_Device(CGraphic_Device::GetInstance())
	,m_pInput_Device(CInput_Device::GetInstance())
	,m_pTimer_Manager(CTimer_Manager::GetInstance())
	,m_pSound_Manager(CSound_Manager::GetInstance())
	,m_pLevel_Manager(CLevel_Manager::GetInstance())
	,m_pObject_Manager(CObject_Manager::GetInstance())
	,m_pComponent_Manager(CComponent_Manager::GetInstance())
	,m_pPipeLine(CPipeLine::GetInstance())
	,m_pLight_Manager(CLight_Manager::GetInstance())
	,m_pTarget_Manager(CTarget_Manager::GetInstance())
	,m_pFrustum(CFrustum::GetInstance())
{
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pInput_Device);
	Safe_AddRef(m_pTimer_Manager);
	Safe_AddRef(m_pSound_Manager);
	Safe_AddRef(m_pLevel_Manager);
	Safe_AddRef(m_pObject_Manager);
	Safe_AddRef(m_pComponent_Manager);
	Safe_AddRef(m_pPipeLine);
	Safe_AddRef(m_pLight_Manager);
	Safe_AddRef(m_pTarget_Manager);
	Safe_AddRef(m_pFrustum);
}

HRESULT CGameInstance::Initialize_Engine(HINSTANCE hInst, _uint iNumLevels, CGraphic_Device::GRAPHICDESC GraphicDesc, ID3D11Device ** ppDeviceOut, ID3D11DeviceContext ** ppDeviceContextOut)
{
	if (nullptr == m_pGraphic_Device)
	{
		MSG_BOX(L"Failed To CGameInstance : Initialize_Engine : nullptr == m_pGraphic_Device");
		return E_FAIL;
	}
	if (FAILED(m_pGraphic_Device->Ready_Graphic_Device(GraphicDesc.hWnd, GraphicDesc.eWinMode, GraphicDesc.iWinCX, GraphicDesc.iWinCY, ppDeviceOut, ppDeviceContextOut)))
	{
		MSG_BOX(L"Failed To CGameInstance : Initialize_Engine : m_pGraphic_Device->Ready_Graphic_Device");
		return E_FAIL;
	}

	if (nullptr == m_pInput_Device)
	{
		MSG_BOX(L"Failed To CGameInstance : Initialize_Engine : nullptr == m_pInput_Device");
		return E_FAIL;
	}
	if (FAILED(m_pInput_Device->InitDevice(hInst, GraphicDesc.hWnd)))
	{
		MSG_BOX(L"Failed To CGameInstance : Initialize_Engine : m_pInput_Device->InitDevice");
		return E_FAIL;
	}

	if (nullptr == m_pObject_Manager)
	{
		MSG_BOX(L"Failed To CGameInstance : Initialize_Engine : nullptr == m_pObject_Manager");
		return E_FAIL;
	}
	if (FAILED(m_pObject_Manager->Reserve_Manager(iNumLevels)))
	{
		MSG_BOX(L"Failed To CGameInstance : Initialize_Engine : m_pObject_Manager->Reserve_Manager");
		return E_FAIL;
	}

	if (nullptr == m_pComponent_Manager)
	{
		MSG_BOX(L"Failed To CGameInstance : Initialize_Engine : nullptr == m_pComponent_Manager");
		return E_FAIL;
	}
	if (FAILED(m_pComponent_Manager->Reserve_Manager(iNumLevels)))
	{
		MSG_BOX(L"Failed To CGameInstance : Initialize_Engine : m_pComponent_Manager->Reserve_Manager");
		return E_FAIL;
	}

	if (nullptr == m_pLight_Manager)
	{
		MSG_BOX(L"Failed To CGameInstance : Initialize_Engine : nullptr == m_pLight_Manager");
		return E_FAIL;
	}
	if (FAILED(m_pLight_Manager->NativeConstruct(*ppDeviceOut, *ppDeviceContextOut)))
	{
		MSG_BOX(L"Failed To CGameInstance : Initialize_Engine : m_pLight_Manager->NativeConstruct");
		return E_FAIL;
	}
	return S_OK;
}

void CGameInstance::Tick_Engine(_double TimeDelta)
{
	if (nullptr == m_pLevel_Manager || nullptr == m_pInput_Device)
	{
		return;
	}
	m_pInput_Device->SetUp_InputDeviceState();

	m_pObject_Manager->Tick(TimeDelta);

	m_pPipeLine->Tick();
	m_pFrustum->Transform_ToWorldSpace();

	m_pObject_Manager->LateTick(TimeDelta);

	m_pLevel_Manager->Tick(TimeDelta);
}

HRESULT CGameInstance::Render_Engine()
{
	if (nullptr == m_pLevel_Manager)
	{
		MSG_BOX(L"Failed To CGameInstance : Render_Engine : nullptr == m_pLevel_Manager");
		return E_FAIL;
	}
	m_pLevel_Manager->Render();
	return S_OK;
}

HRESULT CGameInstance::Render_Begin(_float4 vClearColor)
{
	if (nullptr == m_pGraphic_Device)
	{
		MSG_BOX(L"Failed To CGameInstance : Render_Begin : nullptr == m_pGraphic_Device");
		return E_FAIL;
	}
	m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);
	m_pGraphic_Device->Clear_DepthStencil_View();
	return S_OK;
}

HRESULT CGameInstance::Render_End()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSG_BOX(L"Failed To CGameInstance : Render_End : nullptr == m_pGraphic_Device");
		return E_FAIL;
	}
	return m_pGraphic_Device->Present();
}

_bool CGameInstance::Key_Pressing(_uchar byKeyID)
{
	if (nullptr == m_pInput_Device)
	{
		MSG_BOX(L"Failed To CGameInstance : Key_Pressing : nullptr == m_pInput_Device");
		return false;
	}
	return m_pInput_Device->Key_Pressing(byKeyID);
}

_bool CGameInstance::Key_Down(_uchar byKeyID)
{
	if (nullptr == m_pInput_Device)
	{
		MSG_BOX(L"Failed To CGameInstance : Key_Down : nullptr == m_pInput_Device");
		return false;
	}
	return m_pInput_Device->Key_Down(byKeyID);
}

_bool CGameInstance::Key_Up(_uchar byKeyID)
{
	if (nullptr == m_pInput_Device)
	{
		MSG_BOX(L"Failed To CGameInstance : Key_Up : nullptr == m_pInput_Device");
		return false;
	}
	return m_pInput_Device->Key_Up(byKeyID);
}

_bool CGameInstance::Button_Pressing(CInput_Device::MOUSEBUTTONSTATE eDIMBState)
{
	if (nullptr == m_pInput_Device)
	{
		MSG_BOX(L"Failed To CGameInstance : Button_Pressing : nullptr == m_pInput_Device");
		return false;
	}
	return m_pInput_Device->Button_Pressing(eDIMBState);
}

_bool CGameInstance::Button_Down(CInput_Device::MOUSEBUTTONSTATE eDIMBState)
{
	if (nullptr == m_pInput_Device)
	{
		MSG_BOX(L"Failed To CGameInstance : Button_Down : nullptr == m_pInput_Device");
		return false;
	}
	return m_pInput_Device->Button_Down(eDIMBState);
}

_bool CGameInstance::Button_Up(CInput_Device::MOUSEBUTTONSTATE eDIMBState)
{
	if (nullptr == m_pInput_Device)
	{
		MSG_BOX(L"Failed To CGameInstance : Button_Up : nullptr == m_pInput_Device");
		return false;
	}
	return m_pInput_Device->Button_Up(eDIMBState);
}

HRESULT CGameInstance::Add_Timers(const _tchar* pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
	{
		MSG_BOX(L"Failed To CGameInstance : Add_Timers : nullptr == m_pTimer_Manager");
		return E_FAIL;
	}
	return m_pTimer_Manager->Add_Timers(pTimerTag);
}

_double CGameInstance::Compute_TimeDelta(const _tchar* pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
	{
		return 0.0;
	}
	return m_pTimer_Manager->Compute_TimeDelta(pTimerTag);
}

HRESULT CGameInstance::Init_Sound(const _tchar* pSoundFilePath)
{
	if (nullptr == m_pSound_Manager)
	{
		MSG_BOX(L"Failed To CGameInstance : Init_Sound : nullptr == m_pSound_Manager");
		return E_FAIL;
	}
	return m_pSound_Manager->NativeConstruct(pSoundFilePath);
}

void CGameInstance::Play_Sound(TCHAR* pSoundKey, CSound_Manager::CHANNELID eID, _float _vol)
{
	if (nullptr == m_pSound_Manager)
	{
		MSG_BOX(L"Failed To CGameInstance : Play_Sound : nullptr == m_pSound_Manager");
		return;
	}
	m_pSound_Manager->Play_Sound(pSoundKey, eID, _vol);
}

HRESULT CGameInstance::Open_Level(_uint iLevelIndex, CLevel* pLevel)
{
	if (nullptr == m_pLevel_Manager)
	{
		MSG_BOX(L"Failed To CGameInstance : Open_Level : nullptr == m_pLevel_Manager");
		return E_FAIL;
	}
	return m_pLevel_Manager->OpenLevel(iLevelIndex, pLevel);
}

HRESULT CGameInstance::Clear_LevelResource(_uint iLevelIndex)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_BOX(L"Failed To CGameInstance : Clear_LevelResource : nullptr == m_pObject_Manager");
		return E_FAIL;
	}
	if (FAILED(m_pObject_Manager->Clear(iLevelIndex)))
	{
		MSG_BOX(L"Failed To CGameInstance : Clear_LevelResource : m_pObject_Manager->Clear");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CGameInstance::Release_CurrentLevel()
{
	if (nullptr == m_pLevel_Manager)
	{
		MSG_BOX(L"Failed To CGameInstance : Open_Level : nullptr == m_pLevel_Manager");
		return E_FAIL;
	}
	m_pLevel_Manager->Free();
	return S_OK;
}

CComponent* CGameInstance::Get_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_BOX(L"Failed To CGameInstance : Get_Component : nullptr == m_pObject_Manager");
		return nullptr;
	}
	return m_pObject_Manager->Get_Component(iLevelIndex, pLayerTag, pComponentTag, iIndex);
}

HRESULT CGameInstance::Add_Prototype(const _tchar* pPrototypeTag, CGameObject* pPrototype)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_BOX(L"Failed To CGameInstance : Add_Prototype : nullptr == m_pObject_Manager");
		return E_FAIL;
	}
	return m_pObject_Manager->Add_Prototype(pPrototypeTag, pPrototype);
}

CGameObject* CGameInstance::Add_GameObjectToLayer(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_BOX(L"Failed To CGameInstance : Add_GameObjectToLayer : nullptr == m_pObject_Manager");
		return nullptr;
	}
	return m_pObject_Manager->Add_GameObjectToLayer(iLevelIndex, pLayerTag, pPrototypeTag, pArg);
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, CComponent* pPrototype)
{
	if (nullptr == m_pComponent_Manager)
	{
		MSG_BOX(L"Failed To CGameInstance : Add_Prototype : nullptr == m_pComponent_Manager");
		return E_FAIL;
	}
	return m_pComponent_Manager->Add_Prototype(iLevelIndex, pPrototypeTag, pPrototype);
}

CComponent* CGameInstance::Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg)
{
	if (nullptr == m_pComponent_Manager)
	{
		MSG_BOX(L"Failed To CGameInstance : Clone_Component : nullptr == m_pComponent_Manager");
		return nullptr;
	}
	return m_pComponent_Manager->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
}

_matrix CGameInstance::Get_TransformMatrix(CPipeLine::TRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
	{
		MSG_BOX(L"Failed To CGameInstance : Get_TransformMatrix : nullptr == m_pPipeLine");
		return XMMatrixIdentity();
	}
	return m_pPipeLine->Get_TransformMatrix(eState);
}

_float4x4 CGameInstance::Get_TransformFloat4x4_TP(CPipeLine::TRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
	{
		MSG_BOX(L"Failed To CGameInstance : Get_TransformFloat4x4_TP : nullptr == m_pPipeLine");
		return _float4x4();
	}
	return m_pPipeLine->Get_TransformFloat4x4_TP(eState);
}

_vector CGameInstance::Get_CamPosition()
{
	if (nullptr == m_pPipeLine)
	{
		MSG_BOX(L"Failed To CGameInstance : Get_CamPosition : nullptr == m_pPipeLine");
		return XMVectorSet(0.f, 0.f, 0.f, 1.f);
	}
	return m_pPipeLine->Get_CamPosition();
}

_float4 CGameInstance::Get_CamPositionFloat4()
{
	if (nullptr == m_pPipeLine)
	{
		MSG_BOX(L"Failed To CGameInstance : Get_CamPositionFloat4 : nullptr == m_pPipeLine");
		return _float4(0.f, 0.f, 0.f, 1.f);
	}
	return m_pPipeLine->Get_CamPositionFloat4();
}

const LIGHTDESC* CGameInstance::Get_LightDesc(_uint iIndex)
{
	if (nullptr == m_pLight_Manager)
	{
		MSG_BOX(L"Failed To CGameInstance : Get_LightDesc : nullptr == m_pLight_Manager");
		return nullptr;
	}
	return m_pLight_Manager->Get_LightDesc(iIndex);
}

HRESULT CGameInstance::Add_Lights(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const LIGHTDESC& LightDesc)
{
	if (nullptr == m_pLight_Manager)
	{
		MSG_BOX(L"Failed To CGameInstance : Add_Lights : nullptr == m_pLight_Manager");
		return E_FAIL;
	}
	return m_pLight_Manager->Add_Lights(pDevice, pDeviceContext, LightDesc);
}

ID3D11ShaderResourceView* CGameInstance::Get_RenderTargetSRV(const _tchar* pTargetTag) const
{
	if (nullptr == m_pTarget_Manager)
	{
		MSG_BOX(L"Failed To CGameInstance : Get_RenderTargetSRV : nullptr == m_pTarget_Manager");
		return nullptr;
	}
	return m_pTarget_Manager->Get_SRV(pTargetTag);
}

_bool CGameInstance::isInFrustum_WorldSpace(_vector vWorldPos, _float fRange)
{
	if (nullptr == m_pFrustum)
	{
		MSG_BOX(L"Failed To CGameInstance : isInFrustum_WorldSpace : nullptr == m_pFrustum");
		return false;
	}
	return m_pFrustum->isIn_WorldSpace(vWorldPos, fRange);
}

_bool CGameInstance::isIn_LocalSpace(CTransform* pTransform, _float fRange)
{
	if (nullptr == m_pFrustum)
	{
		MSG_BOX(L"Failed To CGameInstance : isIn_LocalSpace : nullptr == m_pFrustum");
		return false;
	}
	_vector vLocalPos = XMVector3TransformCoord(pTransform->Get_State(CTransform::STATE_POSITION), pTransform->Get_WorldMatrixInverse());
	return m_pFrustum->isIn_WorldSpace(vLocalPos, fRange);
}

void CGameInstance::Release_Engine()
{
	CGameInstance::GetInstance()->DestroyInstance();
	CFrustum::GetInstance()->DestroyInstance();
	CTarget_Manager::GetInstance()->DestroyInstance();
	CLight_Manager::GetInstance()->DestroyInstance();
	CPipeLine::GetInstance()->DestroyInstance();
	CLevel_Manager::GetInstance()->DestroyInstance();
	CObject_Manager::GetInstance()->DestroyInstance();
	CComponent_Manager::GetInstance()->DestroyInstance();
	CSound_Manager::GetInstance()->DestroyInstance();
	CTimer_Manager::GetInstance()->DestroyInstance();
	CInput_Device::GetInstance()->DestroyInstance();
	CGraphic_Device::GetInstance()->DestroyInstance();
}

void CGameInstance::Free()
{
	Safe_Release(m_pFrustum);
	Safe_Release(m_pTarget_Manager);
	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pSound_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pGraphic_Device);
}