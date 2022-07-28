#include "..\public\Timer.h"

CTimer::CTimer()
{
}

HRESULT CTimer::Ready_Timer()
{
	QueryPerformanceCounter(&m_FrameTime);
	QueryPerformanceCounter(&m_FixTime);
	QueryPerformanceCounter(&m_LastTime);
	QueryPerformanceFrequency(&m_CpuTick);
	return S_OK;
}

_double CTimer::Compute_TimeDelta() 
{
	QueryPerformanceCounter(&m_FrameTime);
	if (m_FrameTime.QuadPart - m_FixTime.QuadPart >= m_CpuTick.QuadPart)
	{
		m_FixTime = m_FrameTime;
		QueryPerformanceFrequency(&m_CpuTick);
	}
	m_TimeDelta = (_double(m_FrameTime.QuadPart) - m_LastTime.QuadPart) / m_CpuTick.QuadPart;
	m_LastTime = m_FrameTime;
	return _double(m_TimeDelta);
}

CTimer* CTimer::Create()
{
	CTimer* pInstance = new CTimer();
	if (FAILED(pInstance->Ready_Timer()))
	{
		MSG_BOX(L"Failed To CTimer : Create : pInstance->Ready_Timer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTimer::Free()
{
}