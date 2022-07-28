#pragma once
#include "GameObject.h"
#include "PipeLine.h"

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct tagCameraDesc
	{
		_float3 vEye, vAt, vAxisY;
		_float fFovy, fNear, fFar, fAspect;
	}CAMERADESC;
protected:
	explicit CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
protected:
	CAMERADESC m_CameraDesc;
public:
	virtual CGameObject* Clone(void* pArg) override = 0;
	virtual void Free() override;
};

END