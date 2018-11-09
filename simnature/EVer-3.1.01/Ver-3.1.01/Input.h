/*****************************************************

******************************************************/
#ifndef _INPUT_H_
#define _INPUT_H_

#include <windows.h>
class CInput  
{
public:
	BYTE KeyState(BYTE vKeyCode);
	BOOL IsKeyToggled(BYTE vKeyCode);
	BOOL IsKeyDown(BYTE vKeyCode);
	void Update();
	CInput();
	virtual ~CInput();

private:
   BYTE   m_key[256];
   BOOL   m_mouse[3];
   WPARAM m_mouse_Key;
};
extern CInput g_Input;
#endif