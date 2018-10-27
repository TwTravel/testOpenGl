//////////////////////////////////////////////////////////////////////////////////////////
//	WINDOW_CheckGLError.cpp
//	Check for OpenGL Errors
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

//void WINDOW::CheckGLError - check for an opengl error
void WINDOW::CheckGLError(void)
{
	GLenum error;
	error=glGetError();
	if(!(error==GL_NO_ERROR))
	{
		LOG::Instance()->OutputError("OpenGL Error:");
		if(error==GL_INVALID_ENUM)
		{
			LOG::Instance()->OutputError("	GL_INVALID_ENUM");
			LOG::Instance()->OutputError("	GLenum Argument out of range.");
		}
		if(error==GL_INVALID_VALUE)
		{
			LOG::Instance()->OutputError("	GL_INVALID_VALUE");
			LOG::Instance()->OutputError("	Numeric Argument out of range.");
		}
		if(error==GL_INVALID_OPERATION)
		{
			LOG::Instance()->OutputError("	GL_INVALID_OPERATION");
			LOG::Instance()->OutputError("	Invalid Operation in current state.");
		}
		if(error==GL_STACK_UNDERFLOW)
		{
			LOG::Instance()->OutputError("	GL_STACK_UNDERFLOW");
			LOG::Instance()->OutputError("	Stack Underflow.");
		}
		if(error==GL_STACK_OVERFLOW)
		{
			LOG::Instance()->OutputError("	GL_STACK_OVERFLOW");
			LOG::Instance()->OutputError("	Stack Overflow.");
		}
		if(error==GL_OUT_OF_MEMORY)
		{
			LOG::Instance()->OutputError("	GL_OUT_OF_MEMORY");
			LOG::Instance()->OutputError("	Out of memory.");
		}
	}
}