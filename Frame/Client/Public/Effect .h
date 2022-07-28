#pragma once

#include "Client_Define.h"
#include "GameObject.h"
#include "VIBuffer_Rect_Instance.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CTexture;
class CVIBuffer_Rect_Instance;
END

BEGIN(Client)

class CEffect final : public CGameObject
{
private:
	explicit CEffect(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CEffect(const CEffect& rhs);
	virtual ~CEffect() = default;
	enum EFFECT{ EFFECT_RECT_INSTANCE,EFFECT_TYPE_END };

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

	HRESULT Effect_Set();
	HRESULT Effect_Delete();
	HRESULT Effect_Update(_double TimeDelta);
	HRESULT Buffer_Reset();
private:	
	CRenderer*					m_pRendererCom = nullptr;
	CShader*					m_pShaderCom = nullptr;	
	CTexture*					m_pTextureCom = nullptr;

	CVIBuffer_Rect_Instance*	m_pVIBufferCom = nullptr;


	_uint						m_iKeyFrame = 5;
	_float						m_fMaxTime = 10.f;

	CVIBuffer_Rect_Instance::INSTANCECREATEDESC*	m_CreateDesc = nullptr;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();	

public:
	static CEffect* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END