#include "Timer.h"

DWORD g_ct;

CTimer::CTimer()
{
	m_isRaise = false;
	m_isRun = true;
	m_intval = 300;
}

CTimer::~CTimer()
{

}

void CTimer::SetIntVal(int val)
{
	m_intval = val;
}

bool CTimer::IsRaise()
{
	return m_isRaise&m_isRun;
}

void CTimer::Reset()
{
	m_oldtickt = g_ct;
	m_isRaise  = false;
}

void CTimer::Pause(bool flag)
{
    m_isRun =  flag;
}

void CTimer::Flush()
{
	if(g_ct - m_oldtickt >= m_intval)
	{
		m_isRaise = true;
		m_oldtickt = g_ct;
	}

}

void CTimer::UpdateTime()
{
	g_ct = ::GetTickCount();
}

void CTimer::Continue()
{
	if(m_isRaise)
	{
		m_isRaise = false;
	}
}

DWORD CTimer::GetPassedTime()
{
	return g_ct - m_oldtickt;
}
