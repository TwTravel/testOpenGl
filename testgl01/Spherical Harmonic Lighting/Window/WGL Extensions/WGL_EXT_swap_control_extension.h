//////////////////////////////////////////////////////////////////////////////////////////
//	WGL_EXT_swap_control_extension.h
//	Extension setup header
//	Downloaded from: www.paulsprojects.net
//	Created:	14th November 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef WGL_EXT_SWAP_CONTROL_EXTENSION_H
#define WGL_EXT_SWAP_CONTROL_EXTENSION_H

bool SetUpWGL_EXT_swap_control(const char * wglExtensions);
extern bool WGL_EXT_swap_control_supported;

extern PFNWGLSWAPINTERVALEXTPROC		wglSwapIntervalEXT;
extern PFNWGLGETSWAPINTERVALEXTPROC		wglGetSwapIntervalEXT;

#endif	// WGL_EXT_SWAP_CONTROL_EXTENSION_H