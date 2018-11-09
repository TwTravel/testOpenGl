#include "Input.h"

CInput g_Input;

CInput::CInput()
{

}

CInput::~CInput()
{

}

void CInput::Update()
{
	GetKeyboardState(m_key);	
}

BOOL CInput::IsKeyDown(BYTE vKeyCode)
{
	return m_key[vKeyCode]&0X80;
}

BOOL CInput::IsKeyToggled(BYTE vKeyCode)
{
	return m_key[vKeyCode]&0X01;
}

BYTE CInput::KeyState(BYTE vKeyCode)
{
   return m_key[vKeyCode];
}
