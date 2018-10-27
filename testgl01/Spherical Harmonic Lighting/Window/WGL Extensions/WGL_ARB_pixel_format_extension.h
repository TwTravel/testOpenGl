//////////////////////////////////////////////////////////////////////////////////////////
//	WGL_ARB_pixel_format_extension.h
//	Extension setup header
//	Downloaded from: www.paulsprojects.net
//	Created:	26th August 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef WGL_ARB_PIXEL_FORMAT_EXTENSION_H
#define WGL_ARB_PIXEL_FORMAT_EXTENSION_H

bool SetUpWGL_ARB_pixel_format(const char * wglExtensions);
extern bool WGL_ARB_pixel_format_supported;

extern PFNWGLGETPIXELFORMATATTRIBIVARBPROC			wglGetPixelFormatAttribivARB;
extern PFNWGLGETPIXELFORMATATTRIBFVARBPROC			wglGetPixelFormatAttribfvARB;
extern PFNWGLCHOOSEPIXELFORMATARBPROC				wglChoosePixelFormatARB;

#endif	// WGL_ARB_PIXEL_FORMAT_EXTENSION_H