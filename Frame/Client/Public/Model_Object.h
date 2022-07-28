#pragma once
#include "Client_Define.h"
#include "GameObject.h"
#include "GameInstance.h"

BEGIN(Client)

class CModel_Object abstract : public CGameObject
{
protected:
	CModel_Object(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CModel_Object(const CModel_Object& rhs);
	virtual ~CModel_Object() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta);
	virtual void LateTick(_double TimeDelta);
	virtual HRESULT Render();
private:
	HRESULT SetUp_Renderer();
	HRESULT SetUp_ConstantTable();
protected:
	CGameInstance* m_pGameInstance = nullptr;
protected:
	CRenderer* m_pRenderer = nullptr;
	CShader* m_pShader = nullptr;
	CModel* m_pModel = nullptr;
protected:
	_uint m_iShaderPass = 0;
public:
	virtual CGameObject* Clone(void* pArg) override = 0;
	virtual void Free() override;
};

END