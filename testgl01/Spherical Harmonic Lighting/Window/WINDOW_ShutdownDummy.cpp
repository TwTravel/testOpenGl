//////////////////////////////////////////////////////////////////////////////////////////
//	WINDOW_ShutdownDummy.cpp
//	Shutdown our dummy window
//	Downloaded from: www.paulsprojects.net
//	Created:	27th November 2002
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

//Shutdown dummy window
void WINDOW::ShutdownDummy()
{
	LOG::Instance()->OutputNewLine();

	//Shut down rendering context
	if(hRC)
	{
		//Stop the context from being current
		if(!wglMakeCurrent(NULL, NULL))
			LOG::Instance()->OutputError("Unable to make the Rendering Context non-current");
		else
			LOG::Instance()->OutputSuccess("Rendering Context made non-current");

		//Delete the context
		if(!wglDeleteContext(hRC))
			LOG::Instance()->OutputError("Unable to delete rendering context");
		else
			LOG::Instance()->OutputSuccess("Rendering context deleted");

		hRC=NULL;
	}

	//Release DC
	if(hDC)
	{
		if(!ReleaseDC(hWnd, hDC))
			LOG::Instance()->OutputError("Unable to release device context");
		else
			LOG::Instance()->OutputSuccess("Device Context Released");

		hDC=NULL;
	}

	//Destroy window
	if(hWnd)
	{
		if(!DestroyWindow(hWnd))
			LOG::Instance()->OutputError("Unable to destroy window");
		else
			LOG::Instance()->OutputSuccess("Window Destroyed");

		hWnd=NULL;
	}

	//Unregister class
	if(!UnregisterClass("OpenGL", hInstance))
		LOG::Instance()->OutputError("Unable to unregister class");
	else
		LOG::Instance()->OutputSuccess("Window class unregistered");

	hInstance=NULL;
}
