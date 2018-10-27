//////////////////////////////////////////////////////////////////////////////////////////
//	WINDOW.h
//	Singleton class to handle window, messages, input.
//	Singleton was chosen because of the static callback functions required in WINDOW
//	Downloaded from: www.paulsprojects.net
//	Created:	12th November 2002
//	Modified:	27th November 2002	-	Added "ShutdownDummy"
//									-	Fixed problem with white dialog box
//	Modified:	23rd August 2003	-	Made display of dialog box optional
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef WINDOW_H
#define WINDOW_H

class WINDOW
{
protected:
	//protected constructor and copy constructor to prevent making copies
	WINDOW()
	{}
	WINDOW(const WINDOW &)
	{}
	WINDOW & operator= (const WINDOW &)
	{}

public:
	//public function to access the instance of the window class
	static WINDOW * Instance()
	{
		//Instance of window class
		static WINDOW instance;
		return &instance;
	}

	//Initiate window
	bool Init(	char * windowTitle, int newWidth, int newHeight,
				int newRedBits, int newGreenBits, int newBlueBits, int newAlphaBits,
				int newDepthBits, int newStencilBits,
				bool newFullscreen, bool newVSync, bool dialogBox);

protected:
	//Create a dummy OpenGL window to initialise function pointers
	bool InitDummy();
	void ShutdownDummy();

	//Initiate a standard window
	bool InitStandard();

	//Initiate a window using ARB_pixel_format etc
	bool InitExtended(bool dialogBox);

	//Store whether a certain number of samples is supported
	bool samplesSupported[17];		//0 for no AA, 2-16 for 2-16x AA

	void FindSamplesSupported(void);//fill in the above array


public:
	void Update()
	{	UpdateInput();	}
	void Shutdown();

	bool MakeCurrent(void);		//make this the current GL rendering context
	
	void SwapBuffers()
	{	::SwapBuffers(hDC);	}

	//Handle windows messages
	bool HandleMessages(void);
	static LRESULT CALLBACK WndProc(	HWND hWnd,			//static for CALLBACK
										UINT msg,
										WPARAM wParam,
										LPARAM lParam);

	//Handle dialog box used to select window resolution etc in ARB_extended window setup
	static INT_PTR CALLBACK SelectModeProc(	HWND hWnd,
											UINT msg,
											WPARAM wParam,
											LPARAM lParam);

	//Misc functions
	void CheckGLError(void);
	void SaveScreenshot(void);

public:

	//windows variables
	HGLRC		hRC;			//Rendering context
	HDC			hDC;			//Device context
	HWND		hWnd;			//Window handle
	HINSTANCE	hInstance;		//Handle to window instance

	//Window statistics
	char * title;				//title
	int width, height;			//Window size
	int redBits, greenBits, blueBits, alphaBits;	//color bits
	int depthBits, stencilBits;
	int numSamples;					//number of samples

	bool fullscreen;
	bool vSync;		//is vSync on?



	//Input Functions
	void UpdateInput();

public:
	//Keyboard
	void SetKeyPressed(int keyNumber)
	{
		if(keyNumber>=0 && keyNumber<256)
			keyPressed[keyNumber]=true;
	}
	
	void SetKeyReleased(int keyNumber)
	{
		if(keyNumber>=0 && keyNumber<256)
			keyPressed[keyNumber]=false;
	}

	bool IsKeyPressed(int keyNumber)
	{
		if(keyNumber>=0 && keyNumber<256)
			return keyPressed[keyNumber];
		else
			return false;
	}

protected:
	bool keyPressed[256];	//is a key pressed?

public:
	//Mouse
	void SetLeftButtonPressed()
	{	mouseLDown=true;	}
	
	void SetRightButtonPressed()
	{	mouseRDown=true;	}
	
	void SetLeftButtonReleased()
	{	mouseLDown=false;	}
	
	void SetRightButtonReleased()
	{	mouseRDown=false;	}

	bool IsLeftButtonPressed()
	{	return mouseLDown;	}

	bool IsRightButtonPressed()
	{	return mouseRDown;	}

	int GetMouseX()
	{	return mouseX;	}

	int GetMouseY()
	{	return mouseY;	}

	int GetMouseXMovement()
	{	return mouseX-oldMouseX;	}
	
	int GetMouseYMovement()
	{	return mouseY-oldMouseY;	}

protected:
	int oldMouseX, oldMouseY;
	int mouseX, mouseY;
	bool mouseLDown, mouseRDown;
};

#endif	//WINDOW_H