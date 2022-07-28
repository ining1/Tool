#pragma once
#include "Client_Define.h"
#include "Level.h"
#include "GameInstance.h"

BEGIN(Client)

class CLevel_Game1 final : public CLevel
{
private:
	CLevel_Game1(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CLevel_Game1() = default;
public:
	virtual HRESULT NativeConstruct();
	virtual void Tick(_double TimeDelta);
	virtual HRESULT Render();
public:
	static CLevel_Game1* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual void Free() override;
};

END