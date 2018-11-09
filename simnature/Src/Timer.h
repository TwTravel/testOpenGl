#ifndef _TIMER_H_
#define _TIMER_H_

#include <windows.h>


class CTimer  
{
public:
	CTimer();
	virtual ~CTimer();

public:
	DWORD       GetPassedTime();
	void        Continue();
	static void UpdateTime();
	void        Flush();
	void        Pause(bool flag);
	void        Reset();
	bool        IsRaise();
	void        SetIntVal(int val);
	
private:
	int         m_intval;
	DWORD       m_oldtickt;
	bool        m_isRun;
	bool        m_isRaise;
};

#endif 