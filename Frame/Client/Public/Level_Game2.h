#pragma once
#include "Client_Define.h"
#include "Level.h"
#include "GameInstance.h"

BEGIN(Client)

class CLevel_Game2 final : public CLevel
{
private:
	CLevel_Game2(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CLevel_Game2() = default;
public:
	virtual HRESULT NativeConstruct();
	virtual void Tick(_double TimeDelta);
	virtual HRESULT Render();
public:
	static CLevel_Game2* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual void Free() override;
};

END