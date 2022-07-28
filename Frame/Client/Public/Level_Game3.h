#pragma once
#include "Client_Define.h"
#include "Level.h"
#include "GameInstance.h"

BEGIN(Client)

class CLevel_Game3 final : public CLevel
{
private:
	CLevel_Game3(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CLevel_Game3() = default;
public:
	virtual HRESULT NativeConstruct();
	virtual void Tick(_double TimeDelta);
	virtual HRESULT Render();
public:
	static CLevel_Game3* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual void Free() override;
};

END