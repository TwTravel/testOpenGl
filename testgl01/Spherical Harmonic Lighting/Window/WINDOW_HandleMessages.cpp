//////////////////////////////////////////////////////////////////////////////////////////
//	WINDOW_HandleMessages.cpp
//	Handle Windows messages to our window
//	Downloaded from: www.paulsprojects.net
//	Created:	12th November 2002
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

bool WINDOW::HandleMessages(void)
{
	MSG	msg;	//windows message structure

	//See if there is a message waiting
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		//if so, deal with it
		if(msg.message==WM_QUIT)
			return false;

		//Handle input
		if(msg.message==WM_KEYDOWN)
			SetKeyPressed(msg.wParam);

		if(msg.message==WM_KEYUP)
			SetKeyReleased(msg.wParam);

		if(msg.message==WM_LBUTTONDOWN)
			SetLeftButtonPressed();

		if(msg.message==WM_RBUTTONDOWN)
			SetRightButtonPressed();

		//Mouse button releases are dealt with in WINDOW::Update, so that releases outside
		//the window area are picked up

		//Send message to WINDOW::WndProc
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}


LRESULT CALLBACK WINDOW::WndProc(	HWND hWnd,
									UINT msg,
									WPARAM wParam,
									LPARAM lParam)
{
	switch(msg)
	{
	case WM_SYSCOMMAND:
		{
			switch(wParam)
			{
			case SC_SCREENSAVE:		//Is the screensaver trying to start?
			case SC_MONITORPOWER:	//Is the monitor trying to turn off?
				return 0;			//prevent from happening
			}

			break;
		}

	case WM_CLOSE:	//If we are trying to close the window, post a quit message
		PostQuitMessage(0);
		return 0;
		break;
	}

	//Pass all unhandled messages to DefWindowProc
	return DefWindowProc(hWnd, msg, wParam, lParam);
}




