#pragma once
#include "Client_Define.h"
#include "Base.h"

BEGIN(Client)

class CConsole_Debug final : public CBase
{
	DECLARE_SINGLETON(CConsole_Debug)
private:
	CConsole_Debug();
	virtual ~CConsole_Debug() = default;
public:
	HRESULT NativeConstruct();
	void Tick(_double TimeDelta);
public:
	CRITICAL_SECTION Get_CriticalSection();
private:
	HANDLE m_hThread = 0;
	CRITICAL_SECTION m_CriticalSection;
private:
	_uint i = 0;
public:
	virtual void Free() override;
};

END