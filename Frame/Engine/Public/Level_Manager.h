#pragma once
#include "Base.h"
#include "Level.h"

BEGIN(Engine)

class CLevel_Manager final : public CBase
{
	DECLARE_SINGLETON(CLevel_Manager)
private:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;
public:
	HRESULT OpenLevel(_uint iLevelIndex, CLevel* pLevel);
	void Tick(_double TimeDelta);
	HRESULT Render();
private:
	_uint m_iLevelIndex = 0;
	CLevel* m_pCurrentLevel = nullptr;
public:
	virtual void Free() override;
};

END