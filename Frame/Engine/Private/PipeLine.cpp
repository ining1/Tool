#include "..\Public\PipeLine.h"

IMPLEMENT_SINGLETON(CPipeLine)

CPipeLine::CPipeLine()
{
}

_matrix CPipeLine::Get_TransformMatrix(TRANSFORMSTATE eState)
{
	return XMLoadFloat4x4(&m_TransformMatrix[eState]);
}

_float4x4 CPipeLine::Get_TransformFloat4x4_TP(TRANSFORMSTATE eState)
{
	_float4x4 TransformMatrix;
	XMStoreFloat4x4(&TransformMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformMatrix[eState])));
	return TransformMatrix;
}

_vector CPipeLine::Get_CamPosition()
{
	return XMLoadFloat4(&m_vCamPosition);
}

_float4 CPipeLine::Get_CamPositionFloat4()
{
	return m_vCamPosition;
}

HRESULT CPipeLine::Set_Transform(TRANSFORMSTATE eState, _fmatrix TransformMatrix)
{
	XMStoreFloat4x4(&m_TransformMatrix[eState], TransformMatrix);
	return S_OK;
}

void CPipeLine::Tick()
{
	XMStoreFloat4(&m_vCamPosition, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_TransformMatrix[D3DTS_VIEW])).r[3]);
}

void CPipeLine::Free()
{
}