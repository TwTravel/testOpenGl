//////////////////////////////////////////////////////////////////////////////////////////
//	WGL_ARB_multisample_extension.cpp
//	WGL_ARB_multisample extension setup
//	Downloaded from: www.paulsprojects.net
//	Created:	14th November 2002
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
#include "WGL_ARB_multisample_extension.h"

bool WGL_ARB_multisample_supported=false;

bool SetUpWGL_ARB_multisample(const char * wglExtensions)
{

	//Check for support
	char * extensionString=(char *)wglExtensions;
	char * extensionName="WGL_ARB_multisample";

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
			WGL_ARB_multisample_supported=true;
		}

		//if not, move on
		extensionString+=distanceToSpace+1;
	}
	

	if(!WGL_ARB_multisample_supported)
	{
		LOG::Instance()->OutputError("WGL_ARB_multisample unsupported!");
		return false;
	}

	LOG::Instance()->OutputSuccess("WGL_ARB_multisample supported!");

	//get function pointers
	//none specified

	return true;
}