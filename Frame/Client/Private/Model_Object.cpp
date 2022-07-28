#include "stdafx.h"
#include "..\Public\Model_Object.h"

CModel_Object::CModel_Object(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
	,m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

CModel_Object::CModel_Object(const CModel_Object& rhs)
	:CGameObject(rhs)
	,m_pGameInstance(rhs.m_pGameInstance)
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CModel_Object::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CModel_Object::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To CModel_Object : NativeConstruct : NativeConstruct");
		return E_FAIL;
	}
	if (FAILED(SetUp_Renderer()))
	{
		MSG_BOX(L"Failed To CModel_Object : NativeConstruct : SetUp_Renderer");
		return E_FAIL;
	}
	return S_OK;
}

void CModel_Object::Tick(_double TimeDelta)
{
}

void CModel_Object::LateTick(_double TimeDelta)
{
}

HRESULT CModel_Object::Render()
{
	if (nullptr == m_pShader || nullptr == m_pModel)
	{
		MSG_BOX(L"Failed To CModel_Object : Render : nullptr == m_pShader || nullptr == m_pModel");
		return E_FAIL;
	}
	if (FAILED(SetUp_ConstantTable()))
	{
		MSG_BOX(L"Failed To CModel_Object : Render : SetUp_ConstantTable");
		return E_FAIL;
	}

	_uint iNumMeshContainers = m_pModel->Get_NumMeshContainer();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pModel->Bind_Material_OnShader(m_pShader, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
		{
			MSG_BOX(L"Failed To CModel_Object : Render : m_pModel->Bind_Material_OnShader");
			return E_FAIL;
		}
		if (FAILED(m_pModel->Render(m_pShader, "g_BoneMatrices", i, m_iShaderPass)))
		{
			MSG_BOX(L"Failed To CModel_Object : Render : m_pModel->Render");
			return E_FAIL;
		}
	}
	return S_OK;
}

HRESULT CModel_Object::SetUp_Renderer()
{
	if (FAILED(__super::SetUp_Component(L"Com_Renderer", LEVEL_STATIC, L"Prototype_Component_Renderer", (CComponent**)&m_pRenderer)))
	{
		MSG_BOX(L"Failed To CModel_Object : SetUp_Renderer : SetUp_Component(Com_Renderer)");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CModel_Object::SetUp_ConstantTable()
{
	if (FAILED(m_pTransform->Bind_WorldMatrixOnShader(m_pShader, "g_WorldMatrix")))
	{
		MSG_BOX(L"Failed To CModel_Object : SetUp_ConstantTable : m_pTransform->Bind_WorldMatrixOnShader");
		return E_FAIL;
	}
	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
	{
		MSG_BOX(L"Failed To CModel_Object : SetUp_ConstantTable : m_pShader->Set_RawValue(g_ViewMatrix)");
		return E_FAIL;
	}
	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
	{
		MSG_BOX(L"Failed To CModel_Object : SetUp_ConstantTable : m_pShader->Set_RawValue(g_ProjMatrix)");
		return E_FAIL;
	}
	return S_OK;
}

void CModel_Object::Free()
{
	__super::Free();
	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
	Safe_Release(m_pModel);
	Safe_Release(m_pGameInstance);
}