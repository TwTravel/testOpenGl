//////////////////////////////////////////////////////////////////////////////////////////
//	WINDOW_MakeCurrent.cpp
//	Make the window current
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

bool WINDOW::MakeCurrent()
{
	if(!wglMakeCurrent(hDC, hRC))
	{
		LOG::Instance()->OutputError("Unable to make window the current context");
		return false;
	}

	return true;
}