//////////////////////////////////////////////////////////////////////////////////////////
//	WINDOW_SaveScreenshot.cpp
//	Save a screenshot
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

//Save a screenshot
void WINDOW::SaveScreenshot()
{
	FILE * file;

	//Calculate the filename to use
	char filename[32];

	for(int i=0; i<1000; ++i)
	{
		sprintf(filename, "screen%03d.tga", i);

		//Try opening this file. If it is not possible, use this filename
		file=fopen(filename, "rb");

		if(!file)
			break;

		//Otherwise, the file exists. Try the next, unless we reached the end
		fclose(file);

		if(i==999)
		{
			LOG::Instance()->OutputError("No space to save screenshot - 0 to 999 exist");
			return;
		}
	}

	LOG::Instance()->OutputSuccess("Saving %s", filename);

	//Uncompressed true color tga header
	GLubyte TGAheader[12]={0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	GLubyte infoHeader[6];

	//Create space for the data
	unsigned char * data=new unsigned char[4*width*height];
	if(!data)
	{
		LOG::Instance()->OutputError("Unable to allocate space for screen data");
		return;
	}

	//read in the data
	//Use the front buffer so we can capture an antialiased screen
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glReadBuffer(GL_BACK);

	//Data needs to be in BGR format
	//Swap B and R
	for(int i=0; i<(int)width*height*4; i+=4)
	{
		//repeated xor to swap bytes
		data[i] ^= data[i+2] ^= data[i] ^= data[i+2];
	}

	//Open the file
	file=fopen(filename, "wb");

	//save header
	fwrite(TGAheader, 1, sizeof(TGAheader), file);

	//Fill in info header
	infoHeader[0]=(width & 0x00FF);
	infoHeader[1]=(width & 0xFF00) >> 8;
	infoHeader[2]=(height & 0x00FF);
	infoHeader[3]=(height & 0xFF00) >> 8;
	infoHeader[4]=32;
	infoHeader[5]=0;

	//Save info header
	fwrite(infoHeader, 1, sizeof(infoHeader), file);

	//save data
	fwrite(data, 1, width*height*4, file);

	fclose(file);

	//delete data
	if(data)
		delete [] data;
	data=NULL;

	LOG::Instance()->OutputSuccess("Saved Screenshot: %s", filename);
	return;
}
