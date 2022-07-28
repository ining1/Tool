#pragma once
#include "Base.h"

BEGIN(Engine)

class CHierarchyNode final : public CBase
{
private:
	CHierarchyNode();
	virtual ~CHierarchyNode() = default;
public:
	HRESULT NativeConstruct(aiNode* pAINode, CHierarchyNode* pParent, _uint iDepth);
	void Update_CombinedTransformationMatrix();
public:
	const char* Get_Name();
	_uint Get_Depth() const;
	_matrix Get_OffsetMatrix() const;
	_matrix Get_CombinedMatrix() const;
public:
	void Set_TransformationMatrix(_fmatrix TransformationMatrix);
	void Set_OffsetMatrix(_fmatrix OffsetMatrix);
private:
	char m_szName[MAX_PATH] = "";
	_uint m_iDepth = 0;
	CHierarchyNode*	m_pParent = nullptr;	
	_float4x4 m_OffsetMatrix;
	_float4x4 m_TransformationMatrix;
	_float4x4 m_CombinedTransformationMatrix;
public:
	static CHierarchyNode* Create(aiNode* pAINode, CHierarchyNode* pParent, _uint iDepth);
	virtual void Free() override;
};

END