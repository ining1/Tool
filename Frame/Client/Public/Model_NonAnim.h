#pragma once
#include "Client_Define.h"
#include "Model_Object.h"

BEGIN(Client)

class CModel_NonAnim abstract : public CModel_Object
{
protected:
	CModel_NonAnim(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CModel_NonAnim(const CModel_NonAnim& rhs);
	virtual ~CModel_NonAnim() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta);
	virtual void LateTick(_double TimeDelta);
	virtual HRESULT Render();
private:
	HRESULT SetUp_Shader();
public:
	virtual CGameObject* Clone(void* pArg) override = 0;
	virtual void Free() override;
};

END