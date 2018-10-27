//////////////////////////////////////////////////////////////////////////////////////////
//	WINDOW_Init.cpp
//	Create an OpenGL window
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
#include "GL/glext.h"
#include "GL/wglext.h"
#include "../Log/LOG.h"
#include "WGL extensions/WGL_ARB_extensions_string_extension.h"
#include "WGL extensions/WGL_ARB_multisample_extension.h"
#include "WGL extensions/WGL_ARB_pixel_format_extension.h"
#include "WGL extensions/WGL_EXT_swap_control_extension.h"
#include "WINDOW.h"

bool WINDOW::Init(	char * windowTitle, int newWidth, int newHeight,
					int newRedBits, int newGreenBits, int newBlueBits, int newAlphaBits,
					int newDepthBits, int newStencilBits,
					bool newFullscreen, bool newVSync, bool dialogBox)
{
	//Set member variables
	title=windowTitle;
	
	width=newWidth;
	height=newHeight;
	redBits=newRedBits;
	greenBits=newGreenBits;
	blueBits=newBlueBits;
	alphaBits=newAlphaBits;
	depthBits=newDepthBits;
	stencilBits=newStencilBits;
	fullscreen=newFullscreen;
	vSync=newVSync;

	LOG::Instance()->OutputSuccess("Creating temporary window");
	LOG::Instance()->OutputNewLine();

	//Init dummy window to get extension pointers
	if(!InitDummy())
		return false;
	
	LOG::Instance()->OutputNewLine();

	bool revertToStandard=false;

	//Initiate the WGL extensions. If not supported, use the standard window
	if(	!SetUpWGL_ARB_extensions_string())
		revertToStandard=true;
		
	//If ARB_extensions_string is supported,
	if(!revertToStandard)
	{
		//Get the WGL extensions string
		const char * wglExtensions;
		wglExtensions=wglGetExtensionsStringARB(hDC);
	
		//If the necessary extensions are not supported, use the standard window
		if(	!SetUpWGL_ARB_pixel_format(wglExtensions)	||
			!SetUpWGL_EXT_swap_control(wglExtensions))
			revertToStandard=true;
	
		//Also initialise ARB_multisample
		SetUpWGL_ARB_multisample(wglExtensions);
	}

	//If we need to revert to standard, shut down the dummy window, Create standard window
	//and return
	if(revertToStandard)
	{
		LOG::Instance()->OutputNewLine();
		LOG::Instance()->OutputSuccess("Destroying temporary window");
	
		ShutdownDummy();

		LOG::Instance()->OutputNewLine();
		LOG::Instance()->OutputSuccess("Replacing with standard, non WGL_extension window");
		LOG::Instance()->OutputNewLine();

		InitStandard();

		return true;
	}

	//If we have reached here, the ARB extensions are supported.
	//Shutdown this window and create a window using ARB_pixel_format etc

	//Find supported AA samples with given color bits, if ARB_multisample is supported
	for(int i=0; i<17; ++i)
		samplesSupported[i]=false;

	if(WGL_ARB_multisample_supported)
	{
		FindSamplesSupported();
	}
	else
	{
		samplesSupported[0]=true;
	}

	//Destroy temporary window
	LOG::Instance()->OutputNewLine();
	LOG::Instance()->OutputSuccess("Destroying temporary window");
	
	ShutdownDummy();

	//Creat WGL extension window
	LOG::Instance()->OutputNewLine();
	LOG::Instance()->OutputSuccess("Replacing with WGL_extension window");
	LOG::Instance()->OutputNewLine();

	if(InitExtended(dialogBox))
		return true;

	//If we reached here, InitExtended failed.
	//Try a standard window as a last resort

	//Destroy extended window
	LOG::Instance()->OutputNewLine();
	LOG::Instance()->OutputSuccess("Extended window creation failed - Destroying");
	
	ShutdownDummy();	//Use ShutdownDummy as we have not hidden the cursor yet.
						//So, we do not want the call to show it

	//Create standard window
	LOG::Instance()->OutputNewLine();
	LOG::Instance()->OutputSuccess("Replacing with standard window");
	LOG::Instance()->OutputNewLine();

	if(InitStandard())
		return true;

	//All window creation attempts failed
	LOG::Instance()->OutputNewLine();
	LOG::Instance()->OutputError("All window Creation attempts failed!");
	Shutdown();
	return false;
}
