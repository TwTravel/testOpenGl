//****************************************************************************
//  Leaf.h: interface for the CLeaf class.
//
//  Copyright (c) Boris J. Wang (e_boris2002@hotmail.com) 2002 - 2003
//  From Institute of Computing Technology, Chinese Academy of Sciences
//                                                Beijing 100871, China
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
//  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
//  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
//  PARTICULAR PURPOSE.
//****************************************************************************

#if !defined(AFX_LEAF_H__E4D037A5_5904_45DB_8AB8_192A530D0F2D__INCLUDED_)
#define AFX_LEAF_H__E4D037A5_5904_45DB_8AB8_192A530D0F2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "gl_helper.h"

#define	LF_SIZENOISESEQLEN	256
#define LF_SIZENOISESEQMASK	((DWORD)0xff)
#define LF_NOISEPOWER	(1.0f)			//MAX = 2.0
#define LF_COLORNOISESCALE	(20.0f)			//>0

class CLeaf  
{
public:
	void Render();
	CLeaf();
	~CLeaf(){}

	GLfloat Expand,Twist,Scale;

	static	float A_M;
	static 	CglMaterial Material;
	static  bool IsRenderShadow;
	static	void InitLeafSizeNoiseSeq(BOOL UnInit = FALSE);
	static	int LastSizeNoiseSeqIndex;

protected:
	static	CglTexture Tex;
	float	SizeNoise;
	static	float* SizeNoiseSeq;
};

#endif // !defined(AFX_LEAF_H__E4D037A5_5904_45DB_8AB8_192A530D0F2D__INCLUDED_)
