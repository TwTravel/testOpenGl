//////////////////////////////////////////////////////////////////////////////////////////
//	WINDOW_InitExtended.cpp
//	Create an OpenGL window using ARB_pixel_format etc
//	Downloaded from: www.paulsprojects.net
//	Created:	13th November 2002
//	Updated:	8th December 2002	-	Changed fAttribs in wglChoosePixelFormat from
//										NULL to a pointer to {0, 0}. This allows the extended 
//										window to be used with older NVidia cards & drivers
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/wglext.h>
#include "../Resources/resource.h"
#include "WGL extensions/WGL_ARB_extensions_string_extension.h"
#include "WGL extensions/WGL_ARB_multisample_extension.h"
#include "WGL extensions/WGL_ARB_pixel_format_extension.h"
#include "WGL extensions/WGL_EXT_swap_control_extension.h"
#include "../Log/LOG.h"
#include "WINDOW.h"

bool WINDOW::InitExtended(bool dialogBox)
{
	//Put up dialog box and find out which number of samples to use, if requested
	if(dialogBox)
		DialogBox(	hInstance, MAKEINTRESOURCE(IDD_RESOLUTION),
					HWND_DESKTOP, SelectModeProc);

	//Create a rect structure for the size/position of the window
	RECT windowRect;
	windowRect.left=0;
	windowRect.right=(long)width;
	windowRect.top=0;
	windowRect.bottom=(long)height;


	//Window class structure
	WNDCLASS wc;

	//Fill in window class struct
	wc.style=			CS_HREDRAW | CS_VREDRAW | CS_OWNDC;		//Style - redraw on move, own DC
	wc.lpfnWndProc=		(WNDPROC) WndProc;						//Wndproc handles messages
	wc.cbClsExtra=		0;
	wc.cbWndExtra=		0;
	wc.hInstance=		hInstance;								//Handle to instance
	wc.hIcon=			LoadIcon(NULL, IDI_WINLOGO);			//Load windows logo icon
	wc.hCursor=			LoadCursor(NULL, IDC_ARROW);			//Load standard cursor
	wc.hbrBackground=	NULL;									//No background required
	wc.lpszMenuName=	NULL;									//No Menu
	wc.lpszClassName=	"OpenGL";								//class name

	//Register window class
	if(!RegisterClass(&wc))
	{
		LOG::Instance()->OutputError("Unable to register window class");
		return false;
	}
	else
		LOG::Instance()->OutputSuccess("Window Class Registered");


	//Switch to fullscreen if required
	if(fullscreen)
	{
		DEVMODE screenSettings;	//device mode
		memset(&screenSettings, 0, sizeof(screenSettings));

		screenSettings.dmSize=sizeof(screenSettings);

		//Set size & color bits
		screenSettings.dmPelsWidth=width;
		screenSettings.dmPelsHeight=height;
		screenSettings.dmBitsPerPel=redBits+greenBits+blueBits+alphaBits;
		screenSettings.dmFields= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		//Try to change to full screen
		if(ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			//If failed, ask whether to run in window
			char * errorText="The Requested Full Screen Mode Is Not Supported By\n Your Video Card. Use Windowed Mode Instead?";
			if(MessageBox(NULL, errorText, title, MB_YESNO | MB_ICONEXCLAMATION)==IDYES)
				fullscreen=false;
			else
			{
				LOG::Instance()->OutputError("Requested full screen mode not supported, quitting...");
				return false;
			}
		}
	}


	//Set window style & extended style
	DWORD style, exStyle;
	if(fullscreen)
	{
		exStyle=WS_EX_APPWINDOW;
		style=WS_POPUP | WS_VISIBLE;	//no border

		//Hide cursor
		ShowCursor(false);
	}
	else
	{
		exStyle=WS_EX_CLIENTEDGE;
		style=WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_VISIBLE;
	}


	//Adjust the window size so that client area is the size requested
	AdjustWindowRectEx(&windowRect, style, false, exStyle);



	//Create Window
	if(!(hWnd=CreateWindowEx(	exStyle,									//window style
								"OpenGL",									//class name
								title,
								WS_CLIPSIBLINGS | WS_CLIPCHILDREN | style,	//style
								0, 0,										//position
								windowRect.right-windowRect.left,			//width
								windowRect.bottom-windowRect.top,			//height
								NULL, NULL,
								hInstance,
								NULL)))
	{
		Shutdown();
		LOG::Instance()->OutputError("Unable to create window");
		return false;
	}



	//Get DC
	if(!(hDC=GetDC(hWnd)))
	{
		//if failed,
		Shutdown();
		LOG::Instance()->OutputError("Cannot get the Device Context");
		return false;
	}
	else
		LOG::Instance()->OutputSuccess("DC Created");


		
	

	//Set a list of attributes for our pixel format
	int attributes[26];

	attributes[0]	=	WGL_RED_BITS_ARB;		//bits
	attributes[1]	=	redBits;
	attributes[2]	=	WGL_GREEN_BITS_ARB;
	attributes[3]	=	greenBits;
	attributes[4]	=	WGL_BLUE_BITS_ARB;
	attributes[5]	=	blueBits;
	attributes[6]	=	WGL_ALPHA_BITS_ARB;
	attributes[7]	=	alphaBits;
	attributes[8]	=	WGL_DEPTH_BITS_ARB;
	attributes[9]	=	depthBits;
	attributes[10]	=	WGL_STENCIL_BITS_ARB;
	attributes[11]	=	stencilBits;
	
	attributes[12]	=	WGL_DRAW_TO_WINDOW_ARB;	//required to be true
	attributes[13]	=	true;
	attributes[14]	=	WGL_SUPPORT_OPENGL_ARB;
	attributes[15]	=	true;
	attributes[16]	=	WGL_DOUBLE_BUFFER_ARB;
	attributes[17]	=	true;
	
	attributes[18]	=	WGL_ACCELERATION_ARB;	//required to be FULL_ACCELERATION_ARB
	attributes[19]	=	WGL_FULL_ACCELERATION_ARB;

	//Set antialias attributes
	if(numSamples>0)
	{
		attributes[20]	=	WGL_SAMPLE_BUFFERS_ARB;	//Multisample
		attributes[21]	=	true;
		attributes[22]	=	WGL_SAMPLES_ARB;
		attributes[23]	=	numSamples;
	}
	else
	{
		attributes[20]	=	0;
		attributes[21]	=	0;
		attributes[22]	=	0;
		attributes[23]	=	0;
	}

	//End the list with (0, 0)
	attributes[24]=0;
	attributes[25]=0;

	//Find the best pixelformat matching out requirements
	unsigned int numFormats;
	int pixelFormat;

	float fAttribs[]={0.0f, 0.0f};

	if(!wglChoosePixelFormatARB(hDC, attributes, fAttribs, 1, &pixelFormat, &numFormats))
	{
		LOG::Instance()->OutputError("Unable to find a suitable pixel format");
		return false;
	}

	//Set pixel format
	if(!SetPixelFormat(hDC, pixelFormat, NULL))
	{
		//if failed,
		Shutdown();
		LOG::Instance()->OutputError("Cannot set the pixel format");
		return false;
	}
	else
		LOG::Instance()->OutputSuccess("Pixel format set");
	
	//Create context
	if(!(hRC=wglCreateContext(hDC)))
	{
		//if failed,
		Shutdown();
		LOG::Instance()->OutputError("Unable to Create context");
		return false;
	}
	else
		LOG::Instance()->OutputSuccess("Context Created");

	//Make current
	if(!MakeCurrent())
	{
		Shutdown();
		return false;
	}
	else
		LOG::Instance()->OutputSuccess("GL rendering context made current");



	//Show window
	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);

	//Set Swap interval
	wglSwapIntervalEXT(vSync ? 1 : 0);



	//Update class variables
	glGetIntegerv(GL_RED_BITS, &redBits);
	glGetIntegerv(GL_GREEN_BITS, &greenBits);
	glGetIntegerv(GL_BLUE_BITS, &blueBits);
	glGetIntegerv(GL_ALPHA_BITS, &alphaBits);
	glGetIntegerv(GL_DEPTH_BITS, &depthBits);
	glGetIntegerv(GL_STENCIL_BITS, &stencilBits);

	//Output these parameters
	LOG::Instance()->OutputNewLine();
	LOG::Instance()->OutputSuccess("Window Created!");
	LOG::Instance()->OutputNewLine();
	LOG::Instance()->OutputSuccess("Window Size: (%d, %d)", width, height);
	LOG::Instance()->OutputSuccess("Color Buffer Bits (R, G, B, A): (%d, %d, %d, %d)",
							redBits, greenBits, blueBits, alphaBits);
	LOG::Instance()->OutputSuccess("Depth Buffer Bits: %d", depthBits);
	LOG::Instance()->OutputSuccess("Stencil Buffer Bits: %d", stencilBits);
	LOG::Instance()->OutputNewLine();

	return true;
}