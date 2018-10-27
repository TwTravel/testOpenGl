//////////////////////////////////////////////////////////////////////////////////////////
//	WINDOW_InitStandard.cpp
//	Create an OpenGL window without using ARB_pixel_format etc
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

bool WINDOW::InitStandard()
{
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



	//Set up pixel format
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	
	//Fill in fields
	pfd.nSize=sizeof(PIXELFORMATDESCRIPTOR);		//size
	pfd.nVersion=1;									//version
	pfd.dwFlags=PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;	//flags
	pfd.iPixelType=PFD_TYPE_RGBA;					//rgba
	
	pfd.cColorBits=redBits+blueBits+greenBits;		//color bits
	pfd.cRedBits=redBits;
	pfd.cGreenBits=greenBits;
	pfd.cBlueBits=blueBits;
	pfd.cAlphaBits=alphaBits;

	pfd.cDepthBits=depthBits;						//depth bits
	pfd.cStencilBits=stencilBits;					//stencil bits

	pfd.iLayerType=PFD_MAIN_PLANE;



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

	//Find a pixel format
	GLuint chosenFormat;

	if(!(chosenFormat=ChoosePixelFormat(hDC, &pfd)))
	{
		//if failed,
		Shutdown();
		LOG::Instance()->OutputError("Unable to find a suitable pixel format");
		return false;
	}
	else
		LOG::Instance()->OutputSuccess("Pixel format found");

	//Set pixel format
	if(!SetPixelFormat(hDC, chosenFormat, &pfd))
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