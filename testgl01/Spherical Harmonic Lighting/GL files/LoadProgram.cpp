//////////////////////////////////////////////////////////////////////////////////////////
//	LoadProgram.cpp
//	Function to load a program (ARB_Vertex_program etc)
//	Downloaded from: www.paulsprojects.net
//	Created:	2nd October 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <fstream>
#include "glee.h"			//header for OpenGL 1.4
#include "../Log/LOG.h"
#include "LoadProgram.h"

bool LoadProgram(GLenum target, char * filename)
{
	//Open file
	std::ifstream programFile(filename, std::ios::in | std::ios::binary);
	if(programFile.fail())
	{
		printf("Unable to open %s\n", filename);
		return false;
	}

	//calculate the size of the file
	programFile.seekg(0, std::ios::end);
	int programSize=programFile.tellg();
	programFile.seekg(0, std::ios::beg);
	
	//allocate memory
	unsigned char * programText=new unsigned char[programSize];
	if(!programText)
	{
		printf("Unable to allocate space for program text for %s\n", filename);
		return false;
	}

	//read file
	programFile.read(reinterpret_cast<char *>(programText), programSize);
	programFile.close();

	//Send program string to OpenGL
	glProgramStringARB(target, GL_PROGRAM_FORMAT_ASCII_ARB, programSize, programText);
	
	if(programText)
		delete [] programText;
	programText=NULL;

	//Output position of any error
	int programErrorPos;
	glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB, &programErrorPos);
	if(programErrorPos!=-1)
		LOG::Instance()->OutputError("Program error at position %d in %s",
										programErrorPos, filename);

	//Output error/warning messages if any
	const GLubyte * programErrorString=glGetString(GL_PROGRAM_ERROR_STRING_ARB);
	if(strlen((const char *)programErrorString)>0)
	{
		LOG::Instance()->OutputMisc("Program Error String for %s:\n%s", 
										filename, programErrorString);
	}

	//Is the program under native limits? (Not supported by NV_fragment_program)
	if(target!=GL_FRAGMENT_PROGRAM_NV)
	{
		GLint underNativeLimits;
		glGetProgramivARB(target, GL_PROGRAM_UNDER_NATIVE_LIMITS_ARB, &underNativeLimits);
		if(underNativeLimits==0)
			LOG::Instance()->OutputError("%s exceeds native limits", filename);
	}

	//Return false in case of error
	if(programErrorPos!=-1)
		return false;

	return true;
}