#include "stdafx.h"
#include "..\Public\Test_Model.h"

CTest_Model::CTest_Model(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CModel_Anim(pDevice, pDeviceContext)
{
}

CTest_Model::CTest_Model(const CTest_Model& rhs)
	:CModel_Anim(rhs)
{
}

HRESULT CTest_Model::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To CTest_Model : NativeConstruct_Prototype : NativeConstruct_Prototype");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CTest_Model::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To CTest_Model : NativeConstruct : NativeConstruct");
		return E_FAIL;
	}
	if (FAILED(SetUp_Model()))
	{
		MSG_BOX(L"Failed To CTest_Model : NativeConstruct : SetUp_Model");
		return E_FAIL;
	}
	return S_OK;
}

void CTest_Model::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
}

void CTest_Model::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT CTest_Model::Render()
{
	if (FAILED(__super::Render()))
	{
		MSG_BOX(L"Failed To CTest_Model : Render : Render");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CTest_Model::SetUp_Model()
{
	if (FAILED(__super::SetUp_Component(L"Com_Model", LEVEL_TEST, L"Prototype_Component_Model_Test", (CComponent**)&m_pModel)))
	{
		MSG_BOX(L"Failed To CTest_Model : SetUp_Model : SetUp_Component(Com_Model)");
		return E_FAIL;
	}
	return S_OK;
}

CTest_Model* CTest_Model::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CTest_Model* pInstance = new CTest_Model(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To CTest_Model : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CTest_Model::Clone(void * pArg)
{
	CTest_Model* pInstance = new CTest_Model(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To CTest_Model : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTest_Model::Free()
{
	__super::Free();
}
