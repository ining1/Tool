#pragma once
#include "Base.h"

BEGIN(Engine)

class CTimer final : public CBase
{
private:
	explicit CTimer();
	virtual ~CTimer() = default;
public:
	HRESULT Ready_Timer();
	_double Compute_TimeDelta();
private:
	_double	m_TimeDelta = 0.0;
private:
	LARGE_INTEGER m_FrameTime;
	LARGE_INTEGER m_FixTime;
	LARGE_INTEGER m_LastTime;
	LARGE_INTEGER m_CpuTick;
public:
	static CTimer* Create();
	virtual void Free();
};

END