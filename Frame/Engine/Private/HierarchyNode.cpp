#include "..\Public\HierarchyNode.h"

CHierarchyNode::CHierarchyNode()
{
}

HRESULT CHierarchyNode::NativeConstruct(aiNode* pAINode, CHierarchyNode* pParent, _uint iDepth)
{
	strcpy_s(m_szName, pAINode->mName.data);
	m_pParent = pParent;
	memcpy(&m_TransformationMatrix, &pAINode->mTransformation, sizeof(_float4x4));	
	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));
	m_iDepth = iDepth;
	return S_OK;
}

void CHierarchyNode::Update_CombinedTransformationMatrix()
{
	if (nullptr != m_pParent)
	{
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&m_pParent->m_CombinedTransformationMatrix));
	}
	else
	{
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix));
	}
}

const char* CHierarchyNode::Get_Name()
{
	return m_szName;
}

_uint CHierarchyNode::Get_Depth() const
{
	return m_iDepth;
}

_matrix CHierarchyNode::Get_OffsetMatrix() const
{
	return XMLoadFloat4x4(&m_OffsetMatrix);
}

_matrix CHierarchyNode::Get_CombinedMatrix() const
{
	return XMLoadFloat4x4(&m_CombinedTransformationMatrix);
}

void CHierarchyNode::Set_TransformationMatrix(_fmatrix TransformationMatrix)
{
	XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
}

void CHierarchyNode::Set_OffsetMatrix(_fmatrix OffsetMatrix)
{
	XMStoreFloat4x4(&m_OffsetMatrix, OffsetMatrix);
}

CHierarchyNode* CHierarchyNode::Create(aiNode* pAINode, CHierarchyNode* pParent, _uint iDepth)
{
	CHierarchyNode*	pInstance = new CHierarchyNode();
	if (FAILED(pInstance->NativeConstruct(pAINode, pParent, iDepth)))
	{
		MSG_BOX(L"CHierarchyNode -> Create -> pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHierarchyNode::Free()
{
}