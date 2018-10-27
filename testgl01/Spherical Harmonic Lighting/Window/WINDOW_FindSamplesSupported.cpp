//////////////////////////////////////////////////////////////////////////////////////////
//	WINDOW_FindSamplesSupported.cpp
//	Find which AA samples are supported with the given color bits, for an ARB window
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
#include <GL/glext.h>
#include <GL/wglext.h>
#include "../Log/LOG.h"
#include "WGL extensions/WGL_ARB_extensions_string_extension.h"
#include "WGL extensions/WGL_ARB_multisample_extension.h"
#include "WGL extensions/WGL_ARB_pixel_format_extension.h"
#include "WGL extensions/WGL_EXT_swap_control_extension.h"
#include "WINDOW.h"

//Fill in the samplesSupported array
void WINDOW::FindSamplesSupported()
{
	int attributes[12];
	int results[12];

	//Find out how many PFDs there are
	attributes[0]=WGL_NUMBER_PIXEL_FORMATS_ARB;
	wglGetPixelFormatAttribivARB(hDC, 1, 0, 1, attributes, results);
	int numPfds=results[0];

	//A list of attributes to check for each pixel format
	attributes[0]	=	WGL_RED_BITS_ARB;		//bits
	attributes[1]	=	WGL_GREEN_BITS_ARB;
	attributes[2]	=	WGL_BLUE_BITS_ARB;
	attributes[3]	=	WGL_ALPHA_BITS_ARB;
	attributes[4]	=	WGL_DEPTH_BITS_ARB;
	attributes[5]	=	WGL_STENCIL_BITS_ARB;
	
	attributes[6]	=	WGL_DRAW_TO_WINDOW_ARB;	//required to be true
	attributes[7]	=	WGL_SUPPORT_OPENGL_ARB;
	attributes[8]	=	WGL_DOUBLE_BUFFER_ARB;
	
	attributes[9]	=	WGL_ACCELERATION_ARB;	//required to be FULL_ACCELERATION_ARB

	attributes[10]	=	WGL_SAMPLE_BUFFERS_ARB;	//Multisample
	attributes[11]	=	WGL_SAMPLES_ARB;


	//Loop through all the pixel formats
	for(int i=0; i<numPfds; ++i)
	{
		//Get the attributes
		wglGetPixelFormatAttribivARB(hDC, i+1, 0, 12, attributes, results);

		//See if this format supports the bits required
		if(	results[0]!=redBits		|| results[1]!=greenBits	||
			results[2]!=blueBits	|| results[3]!=alphaBits	||
			results[4]!=depthBits	|| results[5]!=stencilBits)
			continue;

		//Ensure required attributes are true
		if(	results[6]==false	|| results[7]==false		||
			results[8]==false	|| results[9]!=WGL_FULL_ACCELERATION_ARB)
			continue;

		//Save the number of samples in this pixel format
		if(	results[10]==false)
			samplesSupported[0]=true;
		else if(results[11]<=16)						//don't support >16x AA
			samplesSupported[results[11]]=true;
	}
}