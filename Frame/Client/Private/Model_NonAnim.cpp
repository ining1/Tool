#include "stdafx.h"
#include "..\Public\Model_NonAnim.h"

CModel_NonAnim::CModel_NonAnim(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CModel_Object(pDevice, pDeviceContext)
{
}

CModel_NonAnim::CModel_NonAnim(const CModel_NonAnim& rhs)
	:CModel_Object(rhs)
{
}

HRESULT CModel_NonAnim::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To CModel_NonAnim : NativeConstruct_Prototype : NativeConstruct_Prototype");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CModel_NonAnim::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To CModel_NonAnim : NativeConstruct : NativeConstruct");
		return E_FAIL;
	}
	if (FAILED(SetUp_Shader()))
	{
		MSG_BOX(L"Failed To CModel_NonAnim : NativeConstruct : SetUp_Shader");
		return E_FAIL;
	}
	return S_OK;
}

void CModel_NonAnim::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
}

void CModel_NonAnim::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
	m_pRenderer->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CModel_NonAnim::Render()
{
	if (FAILED(__super::Render()))
	{
		MSG_BOX(L"Failed To CModel_NonAnim : Render : Render");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CModel_NonAnim::SetUp_Shader()
{
	if (FAILED(__super::SetUp_Component(L"Com_Shader", LEVEL_STATIC, L"Prototype_Component_Shader_VtxNorTex", (CComponent**)&m_pShader)))
	{
		MSG_BOX(L"Failed To CModel_NonAnim : SetUp_Shader : SetUp_Component(Com_Shader)");
		return E_FAIL;
	}
	return S_OK;
}

void CModel_NonAnim::Free()
{
	__super::Free();
}