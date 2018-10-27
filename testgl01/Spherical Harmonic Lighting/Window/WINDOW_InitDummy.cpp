//////////////////////////////////////////////////////////////////////////////////////////
//	WINDOW_InitDummy.cpp
//	Create a dummy OpenGL window to initialise function pointers
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

//Init dummy window. Small, no fullscreen
bool WINDOW::InitDummy()
{
	//Create a rect structure for the size/position of the window
	RECT windowRect;
	windowRect.left=0;
	windowRect.right=64;
	windowRect.top=0;
	windowRect.bottom=64;


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


	//Set window style & extended style
	DWORD style, exStyle;
	exStyle=WS_EX_CLIENTEDGE;
	style=WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_VISIBLE;

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

	return true;
}