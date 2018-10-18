//****************************************************************************
//  Tree.h: interface for the CTree class.
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

#if !defined(AFX_TREE_H__CFC35265_3CE0_4B6A_8E19_B868294623E5__INCLUDED_)
#define AFX_TREE_H__CFC35265_3CE0_4B6A_8E19_B868294623E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Branch.h"

class CTree  
{
public:
	void DrawShadow(GLfloat Angle);
	void Render();
	bool Initialize();
	void Rebuild(int Depth);
	CTree();
	virtual ~CTree();

	static float TreeDepth;
	static GLfloat TreeShadowLength,BranchShadowDensity,LeafShadowDensity;
	static int TotalVertex,TotalQuad;
	static int TotalNode,TotalMemory;

protected:
	CSegment Base;
	CBranch Root;
};

#endif // !defined(AFX_TREE_H__CFC35265_3CE0_4B6A_8E19_B868294623E5__INCLUDED_)
