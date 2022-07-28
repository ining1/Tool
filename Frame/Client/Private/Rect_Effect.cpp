#include "stdafx.h"
#include "..\Public\Rect_Effect.h"
#include "GameInstance.h"

CRect_Effect::CRect_Effect(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CRect_Effect::CRect_Effect(const CRect_Effect & rhs)
	: CGameObject(rhs)
{
}

HRESULT CRect_Effect::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRect_Effect::NativeConstruct(void * pArg)
{
	

	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;	

	if (FAILED(BufferSet()))
		return E_FAIL;
	
	return S_OK;
}

void CRect_Effect::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	
	if (GetKeyState('E') & 0x8000)
	{
		BufferSet();
	}
}

void CRect_Effect::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
	
	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONLIGHT, this);
		m_pVIBufferCom->Update(TimeDelta);
	}
		
}

HRESULT CRect_Effect::Render()
{
	if (nullptr == m_pShaderCom || 
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pShaderCom->Begin(m_CreateDesc->iShader);

	m_pVIBufferCom->Render();

	
	return S_OK;
}

HRESULT CRect_Effect::BufferSet()
{
	m_CreateDesc = m_pVIBufferCom->Get_CreateDesc();
	m_pVIBufferCom->Restart();
	return S_OK;
}

HRESULT CRect_Effect::SetUp_Components()
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
	if (FAILED(__super::SetUp_Component(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_RectInstance100"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
	m_CreateDesc = m_pVIBufferCom->Get_CreateDesc();


	return S_OK;
}

HRESULT CRect_Effect::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransform->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;	
	if (FAILED(m_pShaderCom->Set_RawValue("g_Color1", &m_CreateDesc->vColor1 , sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_Color2", &m_CreateDesc->vColor2, sizeof(_float4))))
		return E_FAIL;
	_float m_f = m_pVIBufferCom->Get_Time() / m_pVIBufferCom->Get_MaxTime();
	if (FAILED(m_pShaderCom->Set_RawValue("g_Alpha", &m_f, sizeof(_float))))
		return E_FAIL;


	if (FAILED(m_pTextureCom->SetUp_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", m_CreateDesc->iTexture)))
		return E_FAIL;


	
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CRect_Effect * CRect_Effect::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CRect_Effect*	pInstance = new CRect_Effect(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CRect_Effect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRect_Effect::Clone(void * pArg)
{
	CRect_Effect*	pInstance = new CRect_Effect(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CRect_Effect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRect_Effect::Free()
{
	__super::Free();	
	
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
