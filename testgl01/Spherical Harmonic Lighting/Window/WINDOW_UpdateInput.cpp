//////////////////////////////////////////////////////////////////////////////////////////
//	WINDOW_UpdateInput.cpp
//	Update the input per frame
//	Downloaded from: www.paulsprojects.net
//	Created:	13th November 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
#include "../Log/LOG.h"
#include "WINDOW.h"

void WINDOW::UpdateInput()
{
	//Mouse buttons are flagged as pressed by windows messages, so they are
	//only caught when the mouse is in the window.
	//See if any have been released
	if(mouseLDown && !GetAsyncKeyState(VK_LBUTTON))
		mouseLDown=false;

	if(mouseRDown && !GetAsyncKeyState(VK_RBUTTON))
		mouseRDown=false;

	//Update the mouse position
	static POINT mousePosition;
	GetCursorPos(&mousePosition);

	//save last position
	oldMouseX=mouseX;
	oldMouseY=mouseY;

	mouseX=mousePosition.x;
	mouseY=mousePosition.y;
}