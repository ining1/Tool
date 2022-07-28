#pragma once
#include "Client_Define.h"
#include "Level.h"
#include "GameInstance.h"

BEGIN(Client)

class CLevel_Logo final : public CLevel
{
private:
	CLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CLevel_Logo() = default;
public:
	virtual HRESULT NativeConstruct();
	virtual void Tick(_double TimeDelta);
	virtual HRESULT Render();
public:
	static CLevel_Logo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual void Free() override;
};

END