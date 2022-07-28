#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect_Instance final : public CVIBuffer
{
protected:
	CVIBuffer_Rect_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CVIBuffer_Rect_Instance(const CVIBuffer_Rect_Instance& rhs);
	virtual ~CVIBuffer_Rect_Instance() = default;
public:
	typedef struct InstanceDesc
	{
		_float			fSpeed = 1.f;
		_float			fTime = 0.f;
		_float4			vMovement = _float4(0.f, 0.f, 1.f, 0.f);
	}INSTANCEDESC;

	typedef struct InstanceChangeDesc
	{
		_float			fRelaxSpeed = 1.f;
		_float			fSpeed = 0.f;
		_float			fTime = 0.f;
	}INSTANCECHANGEDESC;

	typedef struct InstanceCreateDesc
	{
		_float			fMaxTime = 10.f;
		_uint			iKeyFrame = 5;
		_uint			iTexture = 0;
		_uint			iShader = 0;
		_float			fLastTime = 0;
		_float			fScale = 0.05f;
		_float			fScale_Speed = 5.f;
		_float			fSpeed = 1.f;
		_float			fSpeedRand = 1.f;
		_float4			vMovement = _float4(-0.5f, -0.5f, -0.5f, 0.f);
		_float4			vMovementRand = _float4(1.f, 1.f, 1.f, 0.f);
		_float4			vPostion = _float4(0.f, 0.f, 0.f, 0.f);
		_float4			vPostionRand = _float4(0.f, 0.f, 0.f, 0.f);
		_float4			vColor1 = _float4(0.f, 0.f, 0.f, 0.f);
		_float4			vColor2 = _float4(0.f, 0.f, 0.f, 0.f);
	}INSTANCECREATEDESC;

	//enum INSTANCETYPE { INSTANCETYPE_LOOP, INSTANCETYPE_DELETE, INSTANCETYPE_SPEED, DIRECTION_END };
	virtual HRESULT NativeConstruct_Prototype(_uint iNumInstance);
	virtual HRESULT NativeConstruct(void* pArg);
	
public:
	_bool Update(_double TimeDelta);
	HRESULT Reset();
	HRESULT Restart();
public:
	virtual HRESULT Render() override;

	INSTANCEDESC*		Get_InstanceDesc() { return m_InstanceDesc; }
	INSTANCECHANGEDESC*	Get_ChangeDesc() { return m_ChangeDesc; }
	INSTANCECREATEDESC*	Get_CreateDesc() { return &m_CreateDesc; }
	_uint	Get_KeyFrameNum() { return m_iKeyFrame; }
	_float	Get_MaxTime() { return m_fMaxTime; }
	_float	Get_Time() { return m_fTime; }
private:
	ID3D11Buffer*			m_pVBInstance = nullptr;
	D3D11_BUFFER_DESC		m_VBInstDesc;
	D3D11_SUBRESOURCE_DATA	m_VBInstSubResourceData;

	_uint					m_iInstanceStride = 0;
	_uint					m_iNumInstance = 0;
	_uint					m_iKeyFrame = 0;

	_float					m_fTime = 0.f;
	_float					m_fMaxTime = 10.f;
private:
	INSTANCEDESC*			m_InstanceDesc = nullptr;
	INSTANCECHANGEDESC*		m_ChangeDesc = nullptr;
	INSTANCECREATEDESC		m_CreateDesc;
public:
	static CVIBuffer_Rect_Instance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iNumInstance = 1);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END

