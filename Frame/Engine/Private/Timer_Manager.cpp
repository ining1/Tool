#include "..\public\Timer_Manager.h"

IMPLEMENT_SINGLETON(CTimer_Manager)

CTimer_Manager::CTimer_Manager()
{
}

HRESULT CTimer_Manager::Add_Timers(const _tchar* pTimerTag)
{
	if (nullptr != Find_Timers(pTimerTag))
	{
		MSG_BOX(L"Failed To CTimer_Manager : Add_Timers : nullptr != Find_Timers");
		return E_FAIL;
	}

	CTimer* pTimer = CTimer::Create();
	if (nullptr == pTimer)
	{
		MSG_BOX(L"Failed To CTimer_Manager : Add_Timers : nullptr == pTimer");
		return E_FAIL;
	}
	m_Timers.insert(TIMERS::value_type(pTimerTag, pTimer));
	return S_OK;
}

_double CTimer_Manager::Compute_TimeDelta(const _tchar* pTimerTag)
{
	CTimer* pTimer = Find_Timers(pTimerTag);	
	if (nullptr == pTimer)
	{
		MSG_BOX(L"Failed To CTimer_Manager : Compute_TimeDelta : nullptr == pTimer");
		return E_FAIL;
	}
	return pTimer->Compute_TimeDelta();	
}

CTimer* CTimer_Manager::Find_Timers(const _tchar* pTimerTag)
{
	auto iter = find_if(m_Timers.begin(), m_Timers.end(), CTagFinder(pTimerTag));

	if (iter == m_Timers.end())
	{
		return nullptr;
	}
	return iter->second;
}

void CTimer_Manager::Free()
{
	for (auto& Pair : m_Timers)
	{
		Safe_Release(Pair.second);
	}
	m_Timers.clear();
}