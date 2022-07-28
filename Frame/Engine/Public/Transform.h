#pragma once
#include "Component.h"
#include "Shader.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };
public:
	typedef struct tagTransformDesc
	{
		_double SpeedPerSec;
		_double RotationPerSec;
	}TRANSFORMDESC;
private:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);	
	virtual ~CTransform() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
public:
	_vector Get_State(STATE eState);
	_float3 Get_Scale();
	_matrix Get_WorldMatrix();
	_matrix Get_WorldMatrixInverse();
	_float4x4 Get_WorldFloat4x4();
	_float4x4 Get_WorldFloat4x4_TP();
	TRANSFORMDESC Get_Transform_Desc();
public:
	void Set_State(STATE eState, _fvector vState);
	void Set_TransformDesc(TRANSFORMDESC TransformDesc);
	void Set_WorldMatrix(_fmatrix WorldMatrix);
public:
	HRESULT Bind_WorldMatrixOnShader(class CShader* pShader, const char* pConstantName);
public:
	HRESULT Scaled(_float3 vScale);
private:	
	_float4x4 m_WorldMatrix;
	TRANSFORMDESC m_TransformDesc;
public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END