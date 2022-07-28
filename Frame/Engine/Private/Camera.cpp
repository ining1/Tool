#include "..\Public\Camera.h"

CCamera::CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CCamera::CCamera(const CCamera& rhs)
	:CGameObject(rhs)
{
}

HRESULT CCamera::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CCamera::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To CCamera : NativeConstruct : NativeConstruct");
		return E_FAIL;
	}

	if (nullptr != pArg)
	{
		memcpy(&m_CameraDesc, pArg, sizeof(CAMERADESC));

		_vector vLook = XMLoadFloat3(&m_CameraDesc.vAt) - XMLoadFloat3(&m_CameraDesc.vEye);
		m_pTransform->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook));

		_vector vRight = XMVector3Cross(XMLoadFloat3(&m_CameraDesc.vAxisY), vLook);
		m_pTransform->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight));

		_vector vUp = XMVector3Cross(vLook, vRight);
		m_pTransform->Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp));

		_vector vPosition = XMVectorSetW(XMLoadFloat3(&m_CameraDesc.vEye), 1.f);
		m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);
	}
	return S_OK;
}


void CCamera::Tick(_double TimeDelta)
{
	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);

	_matrix ViewMatrix = m_pTransform->Get_WorldMatrixInverse();
	pPipeLine->Set_Transform(CPipeLine::D3DTS_VIEW, ViewMatrix);

	_matrix ProjMatrix = XMMatrixPerspectiveFovLH(m_CameraDesc.fFovy, m_CameraDesc.fAspect, m_CameraDesc.fNear, m_CameraDesc.fFar);	
	pPipeLine->Set_Transform(CPipeLine::D3DTS_PROJ, ProjMatrix);

	RELEASE_INSTANCE(CPipeLine);
}

void CCamera::LateTick(_double TimeDelta)
{
}

HRESULT CCamera::Render()
{
	return S_OK;
}

void CCamera::Free()
{
	__super::Free();
}