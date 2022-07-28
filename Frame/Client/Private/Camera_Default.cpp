#include "stdafx.h"
#include "..\Public\Camera_Default.h"

CCamera_Default::CCamera_Default(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CCamera(pDevice, pDeviceContext)
{
}

CCamera_Default::CCamera_Default(const CCamera_Default& rhs)
	:CCamera(rhs)
{
}

HRESULT CCamera_Default::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To CCamera_Default : NativeConstruct_Prototype : NativeConstruct_Prototype");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CCamera_Default::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To CCamera_Default : NativeConstruct : NativeConstruct");
		return E_FAIL;
	}
	CTransform::TRANSFORMDESC tTransformDesc;
	ZeroMemory(&tTransformDesc, sizeof(CTransform::TRANSFORMDESC));
	m_pTransform->Set_TransformDesc(tTransformDesc);

	return S_OK;
}

void CCamera_Default::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
}

void CCamera_Default::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT CCamera_Default::Render()
{
	if (FAILED(__super::Render()))
	{
		MSG_BOX(L"Failed To CCamera_Default : Render : Render");
		return E_FAIL;
	}
	return S_OK;
}

CCamera_Default* CCamera_Default::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CCamera_Default* pInstance = new CCamera_Default(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To CCamera_Default : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CCamera_Default::Clone(void* pArg)
{
	CCamera_Default* pInstance = new CCamera_Default(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To CCamera_Default : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCamera_Default::Free()
{
	__super::Free();
}