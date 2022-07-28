#include "stdafx.h"
#include "..\Public\Effect.h"
#include "GameInstance.h"
#include "..\Public\Effect .h"

CEffect::CEffect(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CEffect::CEffect(const CEffect & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffect::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect::NativeConstruct(void * pArg)
{
	

	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;	
	
	return S_OK;
}

void CEffect::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	
	if (GetKeyState('E') & 0x8000)
	{
		Buffer_Reset();
	}
}

void CEffect::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
	
	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONLIGHT, this);
		
		if (m_pVIBufferCom != nullptr)
			m_pVIBufferCom->Update(TimeDelta);	
	}
		
}

HRESULT CEffect::Render()
{
	if (nullptr == m_pShaderCom || 
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	if (m_pVIBufferCom != nullptr)
	{
		m_pShaderCom->Begin(m_CreateDesc->iShader);
		m_pVIBufferCom->Render();
	}
	

	
	return S_OK;
}


HRESULT CEffect::Effect_Set()
{
	if (m_pVIBufferCom != nullptr)
	{
		if (FAILED(__super::SetUp_Component(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_RectInstance100"), (CComponent**)&m_pVIBufferCom)))
			return E_FAIL;
		m_CreateDesc = m_pVIBufferCom->Get_CreateDesc();
	}

	return S_OK;
}

HRESULT CEffect::Effect_Delete()
{
	if (m_pVIBufferCom != nullptr)
	{
		if (FAILED(__super::SetUp_Component(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_RectInstance100"), (CComponent**)&m_pVIBufferCom)))
			return E_FAIL;
		m_CreateDesc = m_pVIBufferCom->Get_CreateDesc();
	}

	return S_OK;
}

HRESULT CEffect::Effect_Update(_double TimeDelta)
{
	return S_OK;
}

HRESULT CEffect::Buffer_Reset()
{
	if (m_pVIBufferCom != nullptr)
	{
		m_pVIBufferCom->Restart();
	}
		







	return S_OK;
}

HRESULT CEffect::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Component(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Component(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxInstance"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Component(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	/* For.Com_VIBuffer */

	
	return S_OK;
}

HRESULT CEffect::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransform->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;	

	if (FAILED(m_pTextureCom->SetUp_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", m_CreateDesc->iTexture)))
		return E_FAIL;


	
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CEffect * CEffect::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CEffect*	pInstance = new CEffect(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEffect::Clone(void * pArg)
{
	CEffect*	pInstance = new CEffect(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect::Free()
{
	__super::Free();	
	
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
