//////////////////////////////////////////////////////////////////////////////////////////
//	WGL_ARB_extensions_string_extension.cpp
//	WGL_ARB_extensions_string extension setup
//	Downloaded from: www.paulsprojects.net
//	Created:	9th September 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#include <windows.h>
#include <GL\gl.h>
#include <GL\glext.h>
#include <GL\wglext.h>
#include "../../Log/LOG.h"
#include "WGL_ARB_extensions_string_extension.h"

bool WGL_ARB_extensions_string_supported=false;

bool SetUpWGL_ARB_extensions_string()
{
	//get function pointer
	wglGetExtensionsStringARB			=	(PFNWGLGETEXTENSIONSSTRINGARBPROC)
											wglGetProcAddress("wglGetExtensionsStringARB");
	
	if(wglGetExtensionsStringARB==NULL)
		WGL_ARB_extensions_string_supported=false;
	else
		WGL_ARB_extensions_string_supported=true;
	
	
	if(!WGL_ARB_extensions_string_supported)
	{
		LOG::Instance()->OutputError("WGL_ARB_extensions_string unsupported!");
		return false;
	}

	LOG::Instance()->OutputSuccess("WGL_ARB_extensions_string supported!");

	return true;
}

//function pointers
PFNWGLGETEXTENSIONSSTRINGARBPROC			wglGetExtensionsStringARB		=NULL;
