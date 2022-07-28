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

class CRect_Effect final : public CGameObject
{
private:
	explicit CRect_Effect(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CRect_Effect(const CRect_Effect& rhs);
	virtual ~CRect_Effect() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

	HRESULT BufferSet();
private:	
	CRenderer*					m_pRendererCom = nullptr;
	CShader*					m_pShaderCom = nullptr;	
	CTexture*					m_pTextureCom = nullptr;
	CVIBuffer_Rect_Instance*	m_pVIBufferCom = nullptr;

	_float						m_fMaxTime = 10.f;
	CVIBuffer_Rect_Instance::INSTANCECREATEDESC*	m_CreateDesc = nullptr;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();	

public:
	static CRect_Effect* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END