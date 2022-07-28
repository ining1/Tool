#include "..\Public\Transform.h"

CTransform::CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CComponent(pDevice, pDeviceContext)
{
}

HRESULT CTransform::NativeConstruct_Prototype()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
	return S_OK;
}

_vector CTransform::Get_State(STATE eState)
{
	return XMLoadFloat4((_float4*)&m_WorldMatrix.m[eState][0]);
}

_float3 CTransform::Get_Scale()
{
	_float3 vScale;
	vScale.x = XMVectorGetX(XMVector3Length(Get_State(STATE_RIGHT)));
	vScale.y = XMVectorGetX(XMVector3Length(Get_State(STATE_UP)));
	vScale.z = XMVectorGetX(XMVector3Length(Get_State(STATE_LOOK)));
	return vScale;
}

_matrix CTransform::Get_WorldMatrix()
{
	return XMLoadFloat4x4(&m_WorldMatrix);
}

_matrix CTransform::Get_WorldMatrixInverse()
{
	return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix));
}

_float4x4 CTransform::Get_WorldFloat4x4()
{
	return m_WorldMatrix;
}

_float4x4 CTransform::Get_WorldFloat4x4_TP()
{
	_float4x4 WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix)));
	return WorldMatrix;
}

CTransform::TRANSFORMDESC CTransform::Get_Transform_Desc()
{
	return m_TransformDesc;
}

void CTransform::Set_State(STATE eState, _fvector vState)
{
	XMStoreFloat4((_float4*)&m_WorldMatrix.m[eState][0], vState);
}

void CTransform::Set_TransformDesc(TRANSFORMDESC TransformDesc)
{
	memcpy(&m_TransformDesc, &TransformDesc, sizeof(TRANSFORMDESC));
}

void CTransform::Set_WorldMatrix(_fmatrix WorldMatrix)
{
	XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);
}

HRESULT CTransform::Bind_WorldMatrixOnShader(CShader* pShader, const char* pConstantName)
{
	if (nullptr == pShader)
	{
		MSG_BOX(L"Failed To CTransform : Bind_WorldMatrixOnShader : nullptr == pShader");
		return E_FAIL;
	}
	_float4x4 WorldMatrixTP;
	XMStoreFloat4x4(&WorldMatrixTP, XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix)));
	pShader->Set_RawValue(pConstantName, &WorldMatrixTP, sizeof(_float4x4));
	return S_OK;
}

HRESULT CTransform::Scaled(_float3 vScale)
{
	return E_NOTIMPL;
}

CTransform* CTransform::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CTransform*	pInstance = new CTransform(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To CTransform : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CTransform::Clone(void* pArg)
{
	return nullptr;
}

void CTransform::Free()
{
	__super::Free();
}