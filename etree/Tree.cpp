//****************************************************************************
//  Tree.cpp: implementation of the CTree class.
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

#include "stdafx.h"
#include "opengl.h"
#include "Tree.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
float CTree::TreeDepth = 19.29;
GLfloat CTree::TreeShadowLength = 0.98;
GLfloat CTree::BranchShadowDensity = 0.30;
GLfloat CTree::LeafShadowDensity = 0.25;

int CTree::TotalVertex,CTree::TotalQuad,CTree::TotalNode,CTree::TotalMemory;


CTree::CTree()
{
}

CTree::~CTree()
{

}

void CTree::Rebuild(int Depth)
{
	CLeaf::LastSizeNoiseSeqIndex = 0;
	Root.LeafScale = CSegment::SegLeafSize;

	Root.Rebuild(Depth);
	if(Root.Segments)
	{
		Root.Segments->Length*=2;
	}
}

bool CTree::Initialize()
{
	Root.Base = &Base;
	Rebuild(TreeDepth);

	CSegment::BarkMTR.SetAmbient(0.15f,0.15f,0.15f);
	CSegment::BarkMTR.SetDiffuse(0.5f,0.5f,0.5f);

//	CLeaf::Material.SetAmbient(0.7,0.652,0.0,1.0);
	CLeaf::Material.SetDiffuse(0.1,0.0,0.0,1.0);

	return true;
}

void CTree::Render()
{
	Root.Render();
}

void CTree::DrawShadow(GLfloat Angle)
{
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	CglTexture::Enable(false);

	glPushMatrix();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glRotatef(90,0,0,1);
		glScalef(0.0,TreeShadowLength,1.0);
		glRotatef(Angle, 0.0f, 1.0f, 0.0f);

		CLeaf::IsRenderShadow = true;
		Render();
		CLeaf::IsRenderShadow = false;

		glDisable(GL_BLEND);
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	CglTexture::Enable();
}
