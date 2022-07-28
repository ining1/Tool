#include "stdafx.h"
#include "..\Public\Console_Debug.h"
#include <iostream>

IMPLEMENT_SINGLETON(CConsole_Debug)

CConsole_Debug::CConsole_Debug()
{
}

unsigned int APIENTRY ThreadEntryFunc_Console_Debug(void* pArg)
{
	CConsole_Debug* pConsole_Debug = (CConsole_Debug*)pArg;
	EnterCriticalSection(&pConsole_Debug->Get_CriticalSection());
	
	char szBuff[MAX_PATH] = "";
	cin.getline(szBuff, MAX_PATH);

	if (!strcmp("deferred off\r", szBuff))
	{
		g_bdebug = false;
	}
	else if (!strcmp("deferred on\r", szBuff))
	{
		g_bdebug = true;
	}

	LeaveCriticalSection(&pConsole_Debug->Get_CriticalSection());
	return 0;
}

HRESULT CConsole_Debug::NativeConstruct()
{
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	InitializeCriticalSection(&m_CriticalSection);

	if (::AllocConsole() == TRUE)
	{
		FILE* nfp[3];
		freopen_s(nfp + 0, "CONIN$", "rb", stdin);
		freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
		freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
		std::ios::sync_with_stdio();
	}

	return S_OK;
}

void CConsole_Debug::Tick(_double TimeDelta)
{
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadEntryFunc_Console_Debug, this, 0, nullptr);
	if (0 == m_hThread)
	{
		MSG_BOX(L"Failed To CConsole_Debug : NativeConstruct : 0 == m_hThread");
		return;
	}
}

CRITICAL_SECTION CConsole_Debug::Get_CriticalSection()
{
	return m_CriticalSection;
}

void CConsole_Debug::Free()
{
	WaitForSingleObject(m_hThread, IGNORE);
	DeleteCriticalSection(&m_CriticalSection);
	DeleteObject(m_hThread);
	CloseHandle(m_hThread);

#ifdef _DEBUG
	FreeConsole();
#endif // _DEBUG
}