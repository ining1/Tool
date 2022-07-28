#include "stdafx.h"
#include "..\Public\Model_Anim.h"

CModel_Anim::CModel_Anim(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CModel_Object(pDevice, pDeviceContext)
{
}

CModel_Anim::CModel_Anim(const CModel_Anim& rhs)
	:CModel_Object(rhs)
{
}

HRESULT CModel_Anim::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To CModel_Anim : NativeConstruct_Prototype : NativeConstruct_Prototype");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CModel_Anim::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To CModel_Anim : NativeConstruct : NativeConstruct");
		return E_FAIL;
	}
	if (FAILED(SetUp_Shader()))
	{
		MSG_BOX(L"Failed To CModel_Anim : NativeConstruct : SetUp_Shader");
		return E_FAIL;
	}
	return S_OK;
}

void CModel_Anim::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	if (m_iCurrentAnim == m_iNextAnim)
	{
		m_pModel->Update(TimeDelta, false);
	}
	else
	{
		m_pModel->Update_NextAnim(TimeDelta, 0, m_iNextAnim, &m_iCurrentAnim);
	}
}

void CModel_Anim::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
	m_pRenderer->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CModel_Anim::Render()
{
	if (FAILED(__super::Render()))
	{
		MSG_BOX(L"Failed To CModel_Anim : Render : Render");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CModel_Anim::SetUp_Shader()
{
	if (FAILED(__super::SetUp_Component(L"Com_Shader", LEVEL_STATIC, L"Prototype_Component_Shader_VtxAnim", (CComponent**)&m_pShader)))
	{
		MSG_BOX(L"Failed To CModel_Anim : SetUp_Shader : SetUp_Component(Com_Shader)");
		return E_FAIL;
	}
	return S_OK;
}

void CModel_Anim::Free()
{
	__super::Free();
}