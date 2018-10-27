//////////////////////////////////////////////////////////////////////////////////////////
//	WGL_ARB_pixel_format_extension.cpp
//	WGL_ARB_pixel_format extension setup
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
#include "WGL_ARB_pixel_format_extension.h"

bool WGL_ARB_pixel_format_supported=false;

bool SetUpWGL_ARB_pixel_format(const char * wglExtensions)
{

	//Check for support
	char * extensionString=(char *)wglExtensions;
	char * extensionName="WGL_ARB_pixel_format";

	char * endOfString;									//store pointer to end of string
	unsigned int distanceToSpace;						//distance to next space

	endOfString=extensionString+strlen(extensionString);

	//loop through string
	while(extensionString<endOfString)
	{
		//find distance to next space
		distanceToSpace=strcspn(extensionString, " ");

		//see if we have found extensionName
		if((strlen(extensionName)==distanceToSpace) &&
			(strncmp(extensionName, extensionString, distanceToSpace)==0))
		{
			WGL_ARB_pixel_format_supported=true;
		}

		//if not, move on
		extensionString+=distanceToSpace+1;
	}
	

	if(!WGL_ARB_pixel_format_supported)
	{
		LOG::Instance()->OutputError("WGL_ARB_pixel_format unsupported!");
		return false;
	}

	LOG::Instance()->OutputSuccess("WGL_ARB_pixel_format supported!");

	//get function pointers
	wglGetPixelFormatAttribivARB			=	(PFNWGLGETPIXELFORMATATTRIBIVARBPROC)
												wglGetProcAddress("wglGetPixelFormatAttribivARB");
	wglGetPixelFormatAttribfvARB			=	(PFNWGLGETPIXELFORMATATTRIBFVARBPROC)
												wglGetProcAddress("wglGetPixelFormatAttribfvARB");
	wglChoosePixelFormatARB					=	(PFNWGLCHOOSEPIXELFORMATARBPROC)
												wglGetProcAddress("wglChoosePixelFormatARB");
	
	return true;
}

//function pointers
PFNWGLGETPIXELFORMATATTRIBIVARBPROC			wglGetPixelFormatAttribivARB		=NULL;
PFNWGLGETPIXELFORMATATTRIBFVARBPROC			wglGetPixelFormatAttribfvARB		=NULL;
PFNWGLCHOOSEPIXELFORMATARBPROC				wglChoosePixelFormatARB				=NULL;

