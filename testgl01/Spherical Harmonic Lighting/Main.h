//////////////////////////////////////////////////////////////////////////////////////////
//	Main.h
//	Spherical Harmonic Lighting
//	Downloaded from: www.paulsprojects.net
//	Created:	2nd October 2003
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef MAIN_H
#define MAIN_H

//Set up GL
bool GLInit();

//Set up variables
bool DemoInit();

//Perform per-frame updates
void UpdateFrame();

//Render a frame
void RenderFrame(double currentTime, double timePassed);

//Shut down demo
void DemoShutdown();

//WinMain
int WINAPI WinMain(	HINSTANCE	hInstance,			//Instance
					HINSTANCE	hPrevInstance,		//Previous Instance
					LPSTR		lpCmdLine,			//Command line params
					int			nShowCmd);			//Window show state

#endif	//MAIN_H