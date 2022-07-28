#include "..\Public\Collider.h"

CCollider::CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CComponent(pDevice, pDeviceContext)
{
}

CCollider::CCollider(const CCollider& rhs)
	:CComponent(rhs)
	,m_eType(rhs.m_eType)
#ifdef _DEBUG
	,m_pBasicEffect(rhs.m_pBasicEffect)
	,m_pBatch(rhs.m_pBatch)
	,m_pInputLayout(rhs.m_pInputLayout)
#endif // _DEBUG
{
#ifdef _DEBUG
	Safe_AddRef(m_pInputLayout);
#endif // _DEBUG
}

HRESULT CCollider::NativeConstruct_Prototype(TYPE eType)
{
	m_eType = eType;

#ifdef _DEBUG
	m_pBasicEffect = new BasicEffect(m_pDevice);
	m_pBasicEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCode = nullptr;
	size_t ShaderByteCodeLength = 0;

	m_pBasicEffect->GetVertexShaderBytecode(&pShaderByteCode, &ShaderByteCodeLength);

	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pDeviceContext);

	if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCode, ShaderByteCodeLength, &m_pInputLayout)))
	{
		MSG_BOX(L"Failed To CCollider : NativeConstruct_Prototype : m_pDevice->CreateInputLayout");
		return E_FAIL;
	}
#endif // _DEBUG
	return S_OK;
}

HRESULT CCollider::NativeConstruct(void* pArg)
{
	COLLIDERDESC ColliderDesc;

	if (nullptr != pArg)
	{
		memcpy(&ColliderDesc, pArg, sizeof(COLLIDERDESC));
	}
	else
	{
		ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
		ColliderDesc.vSize = _float3(1.f, 1.f, 1.f);
		ColliderDesc.fRadius = 1.f;
	}	
	switch (m_eType)
	{
	case CCollider::TYPE_AABB:
		m_pAABB = new BoundingBox(ColliderDesc.vPosition, _float3(ColliderDesc.vSize.x * 0.5f, ColliderDesc.vSize.y * 0.5f, ColliderDesc.vSize.z * 0.5f));
		m_pAABBInWorld = new BoundingBox();
		break;
	case CCollider::TYPE_OBB:
		m_pOBB = new BoundingOrientedBox(ColliderDesc.vPosition, _float3(ColliderDesc.vSize.x * 0.5f, ColliderDesc.vSize.y * 0.5f, ColliderDesc.vSize.z * 0.5f), _float4(0.f, 0.f, 0.f, 1.f));
		m_pOBBInWorld = new BoundingOrientedBox();
		break;
	case CCollider::TYPE_SPHERE:
		m_pSPHERE = new BoundingSphere(ColliderDesc.vPosition, ColliderDesc.fRadius);
		m_pSPHEREInWorld = new BoundingSphere();
		break;
	}
	return S_OK;
}

BoundingBox* CCollider::Get_BoundingBox()
{
	return m_pAABBInWorld;
}

BoundingOrientedBox* CCollider::Get_BoundingOrientedBox()
{
	return m_pOBBInWorld;
}

BoundingSphere* CCollider::Get_BoundingSphere()
{
	return m_pSPHEREInWorld;
}

_bool CCollider::Collision(CCollider* pTargetCollider)
{
	_bool bCollision = false;
	switch (m_eType)
	{
	case TYPE_AABB:
		switch (pTargetCollider->m_eType)
		{
		case TYPE_AABB:
			bCollision = m_pAABBInWorld->Intersects(*(pTargetCollider->m_pAABBInWorld));
			break;
		case TYPE_OBB:
			bCollision = m_pAABBInWorld->Intersects(*(pTargetCollider->m_pOBBInWorld));
			break;
		case TYPE_SPHERE:
			bCollision = m_pAABBInWorld->Intersects(*(pTargetCollider->m_pSPHEREInWorld));
			break;
		}
		break;
	case TYPE_OBB:
		switch (pTargetCollider->m_eType)
		{
		case TYPE_AABB:
			bCollision = m_pOBBInWorld->Intersects(*(pTargetCollider->m_pAABBInWorld));
			break;
		case TYPE_OBB:
			bCollision = m_pOBBInWorld->Intersects(*(pTargetCollider->m_pOBBInWorld));
			break;
		case TYPE_SPHERE:
			bCollision = m_pOBBInWorld->Intersects(*(pTargetCollider->m_pSPHEREInWorld));
			break;
		}
		break;
	case TYPE_SPHERE:
		switch (pTargetCollider->m_eType)
		{
		case TYPE_AABB:
			bCollision = m_pSPHEREInWorld->Intersects(*(pTargetCollider->m_pAABBInWorld));
			break;
		case TYPE_OBB:
			bCollision = m_pSPHEREInWorld->Intersects(*(pTargetCollider->m_pOBBInWorld));
			break;
		case TYPE_SPHERE:
			bCollision = m_pSPHEREInWorld->Intersects(*(pTargetCollider->m_pSPHEREInWorld));
			break;
		}
		break;
	}
	return bCollision;
}

void CCollider::Update(_fmatrix WorldMatrix)
{
	if (TYPE_AABB == m_eType)
	{
		_matrix Matrix = Remove_Rotation(WorldMatrix);
		m_pAABB->Transform(*m_pAABBInWorld, Matrix);
		m_vPosition[0] = _float3(m_pAABBInWorld->Center.x - m_pAABBInWorld->Extents.x, m_pAABBInWorld->Center.y + m_pAABBInWorld->Extents.y, m_pAABBInWorld->Center.z + m_pAABBInWorld->Extents.z);
		m_vPosition[1] = _float3(m_pAABBInWorld->Center.x + m_pAABBInWorld->Extents.x, m_pAABBInWorld->Center.y + m_pAABBInWorld->Extents.y, m_pAABBInWorld->Center.z + m_pAABBInWorld->Extents.z);
		m_vPosition[2] = _float3(m_pAABBInWorld->Center.x + m_pAABBInWorld->Extents.x, m_pAABBInWorld->Center.y - m_pAABBInWorld->Extents.y, m_pAABBInWorld->Center.z + m_pAABBInWorld->Extents.z);
		m_vPosition[3] = _float3(m_pAABBInWorld->Center.x - m_pAABBInWorld->Extents.x, m_pAABBInWorld->Center.y - m_pAABBInWorld->Extents.y, m_pAABBInWorld->Center.z + m_pAABBInWorld->Extents.z);

		m_vPosition[4] = _float3(m_pAABBInWorld->Center.x - m_pAABBInWorld->Extents.x, m_pAABBInWorld->Center.y + m_pAABBInWorld->Extents.y, m_pAABBInWorld->Center.z - m_pAABBInWorld->Extents.z);
		m_vPosition[5] = _float3(m_pAABBInWorld->Center.x + m_pAABBInWorld->Extents.x, m_pAABBInWorld->Center.y + m_pAABBInWorld->Extents.y, m_pAABBInWorld->Center.z - m_pAABBInWorld->Extents.z);
		m_vPosition[6] = _float3(m_pAABBInWorld->Center.x + m_pAABBInWorld->Extents.x, m_pAABBInWorld->Center.y - m_pAABBInWorld->Extents.y, m_pAABBInWorld->Center.z - m_pAABBInWorld->Extents.z);
		m_vPosition[7] = _float3(m_pAABBInWorld->Center.x - m_pAABBInWorld->Extents.x, m_pAABBInWorld->Center.y - m_pAABBInWorld->Extents.y, m_pAABBInWorld->Center.z - m_pAABBInWorld->Extents.z);
	}
	if (TYPE_OBB == m_eType)
	{
		_matrix Matrix = WorldMatrix;
		Matrix.r[3] = XMVectorSetY(Matrix.r[3], 0.f);
		m_pOBB->Transform(*m_pOBBInWorld, WorldMatrix);
		m_vPosition[0] = _float3(m_pOBBInWorld->Center.x - m_pOBBInWorld->Extents.x, m_pOBBInWorld->Center.y + m_pOBBInWorld->Extents.y, m_pOBBInWorld->Center.z + m_pOBBInWorld->Extents.z);
		m_vPosition[1] = _float3(m_pOBBInWorld->Center.x + m_pOBBInWorld->Extents.x, m_pOBBInWorld->Center.y + m_pOBBInWorld->Extents.y, m_pOBBInWorld->Center.z + m_pOBBInWorld->Extents.z);
		m_vPosition[2] = _float3(m_pOBBInWorld->Center.x + m_pOBBInWorld->Extents.x, m_pOBBInWorld->Center.y - m_pOBBInWorld->Extents.y, m_pOBBInWorld->Center.z + m_pOBBInWorld->Extents.z);
		m_vPosition[3] = _float3(m_pOBBInWorld->Center.x - m_pOBBInWorld->Extents.x, m_pOBBInWorld->Center.y - m_pOBBInWorld->Extents.y, m_pOBBInWorld->Center.z + m_pOBBInWorld->Extents.z);

		m_vPosition[4] = _float3(m_pOBBInWorld->Center.x - m_pOBBInWorld->Extents.x, m_pOBBInWorld->Center.y + m_pOBBInWorld->Extents.y, m_pOBBInWorld->Center.z - m_pOBBInWorld->Extents.z);
		m_vPosition[5] = _float3(m_pOBBInWorld->Center.x + m_pOBBInWorld->Extents.x, m_pOBBInWorld->Center.y + m_pOBBInWorld->Extents.y, m_pOBBInWorld->Center.z - m_pOBBInWorld->Extents.z);
		m_vPosition[6] = _float3(m_pOBBInWorld->Center.x + m_pOBBInWorld->Extents.x, m_pOBBInWorld->Center.y - m_pOBBInWorld->Extents.y, m_pOBBInWorld->Center.z - m_pOBBInWorld->Extents.z);
		m_vPosition[7] = _float3(m_pOBBInWorld->Center.x - m_pOBBInWorld->Extents.x, m_pOBBInWorld->Center.y - m_pOBBInWorld->Extents.y, m_pOBBInWorld->Center.z - m_pOBBInWorld->Extents.z);
	}
	if (TYPE_SPHERE == m_eType)
	{
		_matrix Matrix = WorldMatrix;
		Matrix.r[3] = XMVectorSetY(Matrix.r[3], 0.f);
		m_pSPHERE->Transform(*m_pSPHEREInWorld, WorldMatrix);
	}
}

#ifdef _DEBUG
HRESULT CCollider::Render()
{
	_vector vColor = m_isCollision == true ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f);
	vColor = m_bPicking == true ? XMVectorSet(0.f, 0.f, 1.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f);

	_matrix ViewMatrix, ProjMatrix;
	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);

	ViewMatrix = pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);
	ProjMatrix = pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);

	m_pBasicEffect->SetWorld(XMMatrixIdentity());
	m_pBasicEffect->SetView(ViewMatrix);
	m_pBasicEffect->SetProjection(ProjMatrix);

	m_pDeviceContext->IASetInputLayout(m_pInputLayout);
	m_pBasicEffect->Apply(m_pDeviceContext);
	m_pBatch->Begin();

	switch (m_eType)
	{
	case CCollider::TYPE_AABB:
		DX::Draw(m_pBatch, *m_pAABBInWorld, vColor);
		break;
	case CCollider::TYPE_OBB:
		DX::Draw(m_pBatch, *m_pOBBInWorld, vColor);
		break;
	case CCollider::TYPE_SPHERE:
		DX::Draw(m_pBatch, *m_pSPHEREInWorld, vColor);
		break;
	}
	m_pBatch->End();	

	RELEASE_INSTANCE(CPipeLine);
	return S_OK;
}
#endif // _DEBUG

_matrix CCollider::Remove_Rotation(_fmatrix TransformMatrix)
{
	_matrix ResultMatrix = TransformMatrix;
		
	_vector vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(TransformMatrix.r[0]));
	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(TransformMatrix.r[1]));
	_vector vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVectorGetX(XMVector3Length(TransformMatrix.r[2]));

	ResultMatrix.r[0] = vRight;
	ResultMatrix.r[1] = vUp;
	ResultMatrix.r[2] = vLook;

	return ResultMatrix;
}

_vector CCollider::Compute_Min()
{
	if (nullptr == m_pAABBInWorld)
	{
		return XMVectorZero();
	}
	return XMVectorSet(m_pAABBInWorld->Center.x - m_pAABBInWorld->Extents.x, m_pAABBInWorld->Center.y - m_pAABBInWorld->Extents.y, m_pAABBInWorld->Center.z - m_pAABBInWorld->Extents.z, 1.f);
}

_vector CCollider::Compute_Max()
{
	if (nullptr == m_pAABBInWorld)
	{
		return XMVectorZero();
	}
	return XMVectorSet(m_pAABBInWorld->Center.x + m_pAABBInWorld->Extents.x, m_pAABBInWorld->Center.y + m_pAABBInWorld->Extents.y, m_pAABBInWorld->Center.z + m_pAABBInWorld->Extents.z, 1.f);
}

CCollider::OBBDESC CCollider::Compute_OBB()
{
	OBBDESC OBBDesc;
	ZeroMemory(&OBBDesc, sizeof(OBBDESC));

	_float3 vPoints[8];

	m_pOBBInWorld->GetCorners(vPoints);

	XMStoreFloat3(&OBBDesc.vCenter, (XMLoadFloat3(&vPoints[2]) + XMLoadFloat3(&vPoints[4])) * 0.5f);

	XMStoreFloat3(&OBBDesc.vAlignAxis[0], XMVector3Normalize(XMLoadFloat3(&vPoints[2]) - XMLoadFloat3(&vPoints[3])));
	XMStoreFloat3(&OBBDesc.vAlignAxis[1], XMVector3Normalize(XMLoadFloat3(&vPoints[2]) - XMLoadFloat3(&vPoints[1])));
	XMStoreFloat3(&OBBDesc.vAlignAxis[2], XMVector3Normalize(XMLoadFloat3(&vPoints[2]) - XMLoadFloat3(&vPoints[6])));
	
	XMStoreFloat3(&OBBDesc.vCenterAxis[0], (XMLoadFloat3(&vPoints[2]) + XMLoadFloat3(&vPoints[5])) * 0.5f - XMLoadFloat3(&OBBDesc.vCenter));
	XMStoreFloat3(&OBBDesc.vCenterAxis[1], (XMLoadFloat3(&vPoints[2]) + XMLoadFloat3(&vPoints[7])) * 0.5f - XMLoadFloat3(&OBBDesc.vCenter));
	XMStoreFloat3(&OBBDesc.vCenterAxis[2], (XMLoadFloat3(&vPoints[2]) + XMLoadFloat3(&vPoints[0])) * 0.5f - XMLoadFloat3(&OBBDesc.vCenter));

	return OBBDesc;	
}

CCollider* CCollider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, TYPE eType)
{
	CCollider* pInstance = new CCollider(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype(eType)))
	{
		MSG_BOX(L"Failed To CCollider : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CCollider::Clone(void* pArg)
{
	CCollider* pInstance = new CCollider(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To CCollider : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCollider::Free()
{
	__super::Free();

	Safe_Delete(m_pOBBInWorld);
	Safe_Delete(m_pOBB);

	Safe_Delete(m_pSPHEREInWorld);
	Safe_Delete(m_pSPHERE);

	Safe_Delete(m_pAABBInWorld);
	Safe_Delete(m_pAABB);

#ifdef _DEBUG
	Safe_Release(m_pInputLayout);

	if (false == m_isCloned)
	{		
		Safe_Delete(m_pBatch);
		Safe_Delete(m_pBasicEffect);
	}
#endif // _DEBUG
}