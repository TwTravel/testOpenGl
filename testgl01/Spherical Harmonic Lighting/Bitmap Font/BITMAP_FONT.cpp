//////////////////////////////////////////////////////////////////////////////////////////
//	BITMAP_FONT.cpp
//	Functions for a bitmap font class
//	Downloaded from: www.paulsprojects.net
//	Created:	12th November 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "../GL files/glee.h"
#include <GL/glu.h>
#include "../Log/LOG.h"
#include "../Window/WINDOW.h"
#include "BITMAP_FONT.h"

//Initiate font
bool BITMAP_FONT::Init()
{
	HFONT font;

	//Create 96 display lists
	textBase=glGenLists(96);
	if(textBase==0)
	{
		LOG::Instance()->OutputError("Unable to create 96 display lists for font");
		return false;
	}

	//Create font
	font=CreateFont(	-18,			//height
						0,				//default width,
						0, 0,			//angles
						FW_BOLD,		//bold
						false,			//italic
						false,			//underline
						false,			//strikeout
						ANSI_CHARSET,	//character set
						OUT_TT_PRECIS,	//precision
						CLIP_DEFAULT_PRECIS,
						ANTIALIASED_QUALITY,	//quality
						FF_DONTCARE | DEFAULT_PITCH,
						"Courier New");

	//Select font
	SelectObject(WINDOW::Instance()->hDC, font);

	//Fill in the 96 display lists, starting with character 32
	wglUseFontBitmaps(WINDOW::Instance()->hDC, 32, 96, textBase);

	LOG::Instance()->OutputSuccess("Font created successfully");
	
	return true;
}

//Start text mode
void BITMAP_FONT::StartTextMode(void)
{
	//Create a display list if not already done
	if(!startTextModeList)
	{
		startTextModeList=glGenLists(1);
		glNewList(startTextModeList, GL_COMPILE);
		{
			glListBase(textBase-32);
			
			//Set projection matrix
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			gluOrtho2D(0.0f, WINDOW::Instance()->width, WINDOW::Instance()->height, 0.0f);

			//Set modelview matrix
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();

			//Set states
			glDisable(GL_DEPTH_TEST);
		}
		glEndList();
	}

	//Call the list
	glCallList(startTextModeList);
}

//Print some text
void BITMAP_FONT::Print(int x, int y, const char * string, ...)
{
	if(string==NULL)
		return;

	//Convert to text
	static char text[256];

	va_list va;
	
	va_start(va, string);
	vsprintf(text, string, va);
	va_end(va);

	//Print the text
	glRasterPos2i(x, y);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
}

//End text mode
void BITMAP_FONT::EndTextMode(void)
{
	//restore matrices
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	//reset other states
	glListBase(0);
	glEnable(GL_DEPTH_TEST);
}

