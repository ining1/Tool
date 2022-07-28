#pragma once
#include "Client_Define.h"
#include "Model_Anim.h"

BEGIN(Client)

class CTest_Model final : public CModel_Anim
{
private:
	CTest_Model(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CTest_Model(const CTest_Model& rhs);
	virtual ~CTest_Model() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta);
	virtual void LateTick(_double TimeDelta);
	virtual HRESULT Render();
private:
	HRESULT SetUp_Model();
public:
	static CTest_Model* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END