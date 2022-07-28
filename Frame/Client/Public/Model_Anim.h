#pragma once
#include "Client_Define.h"
#include "Model_Object.h"

BEGIN(Client)

class CModel_Anim abstract : public CModel_Object
{
protected:
	CModel_Anim(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CModel_Anim(const CModel_Anim& rhs);
	virtual ~CModel_Anim() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta);
	virtual void LateTick(_double TimeDelta);
	virtual HRESULT Render();
private:
	HRESULT SetUp_Shader();
private:
	_uint m_iCurrentAnim = 0;
	_uint m_iNextAnim = 0;
public:
	virtual CGameObject* Clone(void* pArg) override = 0;
	virtual void Free() override;
};

END