#pragma once
#include "Base.h"
#include "Model.h"
#include "Renderer.h"
#include "Transform.h"
#include "VIBuffer_Rect.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase 
{
protected:
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta);
	virtual void LateTick(_double TimeDelta);
	virtual HRESULT Render();
public:
	CComponent* Get_Component(const _tchar* pComponentTag);
	_bool Get_Death();
	_float Get_DistanceToCamera();
public:
	void Set_Dead(_bool bDead);
protected:
	HRESULT SetUp_Component(const _tchar* pComponentTag, _uint iLevelIndex, const _tchar* pPrototypeTag, CComponent** ppOut, void* pArg = nullptr);
	CComponent* Find_Component(const _tchar* pComponentTag);
protected:
	map<const _tchar*, CComponent*> m_Components;
	typedef map<const _tchar*, CComponent*>	COMPONENTS;
protected:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;
protected:
	CTransform* m_pTransform = nullptr;
protected:
	_bool m_bDeath = false;
	_float m_fDistanceToCamera = 0.f;
public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END