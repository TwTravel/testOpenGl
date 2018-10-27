//////////////////////////////////////////////////////////////////////////////////////////
//	IMAGE_Load8BitBMP.cpp
//	Load an 8 bit paletted .bmp file
//	Downloaded from: www.paulsprojects.net
//	Created:	9th November 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "../GL files/glee.h"
#include "../Log/LOG.h"
#include "IMAGE.h"

//Load an 8 bit .bmp file
bool IMAGE::Load8BitBMP(char * filename)
{
	LOG::Instance()->OutputSuccess("Loading %s in Load8BitBMP()", filename);

	FILE * file;
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;

	//Open file for reading
	file=fopen(filename, "rb");
	if(!file)
	{
		LOG::Instance()->OutputError("Unable to open %s", filename);
		return false;
	}

	//read the file header
	fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, file);

	//Check this is a bitmap
	if(fileHeader.bfType != bitmapID)
	{
		fclose(file);
		LOG::Instance()->OutputError("%s is not a legal .bmp", filename);
		return false;
	}

	//Read in the information header
	fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, file);

	if(infoHeader.biBitCount!=8)
	{
		fclose(file);
		LOG::Instance()->OutputError("%s is not an 8 bit .bmp", filename);
		return false;
	}

	//Set class variables
	bpp=8;
	format=GL_COLOR_INDEX;
	width=infoHeader.biWidth;
	height=infoHeader.biHeight;

	paletted=true;
	paletteBpp=24;
	paletteFormat=GL_RGB;

	//Calculate the stride in bytes for each row (allow for 4-byte padding)
	stride=CalculateStride();

	//Create space for a temporary palette
	GLubyte * tempPalette=new GLubyte[256*4];
	if(!tempPalette)
	{
		fclose(file);
		LOG::Instance()->OutputError("Unable to allocate space for palette");
		return false;
	}

	//Load the palette
	fread(tempPalette, 256*4, 1, file);

	//Create space for palette (disregard needless 4th entry)
	palette=new GLubyte[256*3];
	if(!palette)
	{
		fclose(file);
		LOG::Instance()->OutputError("Unable to allocate space for palette");
		return false;
	}

	//Copy the palette entries from tempPalette to palette
	//swap bytes 0 and 2 to go from BGR to RGB
	for(int i=0; i<256; ++i)
	{
		palette[i*3+0]=tempPalette[i*4+2];
		palette[i*3+1]=tempPalette[i*4+1];
		palette[i*3+2]=tempPalette[i*4+0];
	}

	//Clear "tempPalette" data
	if(tempPalette)
		delete [] tempPalette;
	tempPalette=NULL;

	//Point "file" to the beginning of the data
	fseek(file, fileHeader.bfOffBits, SEEK_SET);

	//Allocate space for the image data
	data=new GLubyte[stride*height];
	if(!data)
	{
		fclose(file);
		LOG::Instance()->OutputError(	"Unable to allocate data for %s of size %d x %d", filename,
								stride, height);
		return false;
	}

	//read in the data
	fread(data, 1, stride*height, file);

	fclose(file);

	LOG::Instance()->OutputSuccess("Loaded %s Correctly!", filename);
	return true;
}
