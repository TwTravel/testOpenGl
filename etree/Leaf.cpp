//****************************************************************************
//  Leaf.cpp: implementation of the CLeaf class.
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
#include "Leaf.h"
#include <math.h>
#include "tree.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CglTexture		CLeaf::Tex(_T("leaf.bmp"),GL_Tex_BMP_RGA_RG0B,8);
CglMaterial		CLeaf::Material;

float CLeaf::A_M = 0.10;
bool CLeaf::IsRenderShadow = false;
int CLeaf::LastSizeNoiseSeqIndex = 0;
float* CLeaf::SizeNoiseSeq = NULL;

void CLeaf::InitLeafSizeNoiseSeq(BOOL UnInit)
{
	if(UnInit)
	{
		if(SizeNoiseSeq)
		{
			delete [] SizeNoiseSeq;
			SizeNoiseSeq = NULL;
		}
	}
	else
	{
		if(!SizeNoiseSeq)
		{
			SizeNoiseSeq = new float[LF_SIZENOISESEQLEN];
			ASSERT(SizeNoiseSeq);
			srand((unsigned)time(NULL));
			for(int i=0;i<LF_SIZENOISESEQLEN;i++)SizeNoiseSeq[i] = (1.0-LF_NOISEPOWER/2.0)+LF_NOISEPOWER*((float)rand())/((float)RAND_MAX);
		}
	}
}

CLeaf::CLeaf()
{
	Scale = 1.0;
	Expand = 0.0;
	Twist = 0.0f;
	if(SizeNoiseSeq)
	{
		SizeNoise = SizeNoiseSeq[(LastSizeNoiseSeqIndex++)&LF_SIZENOISESEQMASK];
	}
}

void CLeaf::Render()
{
	glPushMatrix();
		GLfloat rs = Scale*SizeNoise;
		glScalef(rs,rs,rs);
		glRotatef(Twist,0,1,0);
		glRotatef(Expand,1,0,0);

		Tex.Apply();
		rs = ((SizeNoise-1.0)/LF_COLORNOISESCALE);

		Material.SetAmbient(0.47+rs,0.552-rs,0.0,1.0);
		Material.Apply();

		if(IsRenderShadow)
			glColor4f(0,0,0,CTree::LeafShadowDensity);
		else
			glColor3f(1,1,1);

		if(!IsRenderShadow)
		{
			glAlphaFunc(GL_LESS, A_M);
			glEnable(GL_ALPHA_TEST);
		}

		glBegin(GL_QUADS);
			glNormal3f(1.0f,-1.0f,1.0f);
			glTexCoord2f(1.0,0.0);
			glVertex3f(0.5, 0.0f,0.0f); 

			glNormal3f(1.0f,1.0f,1.0f);
			glTexCoord2f(1.0,1.0); 
			glVertex3f(0.5, 1.0f,0.0f); 

			glNormal3f(-1.0f,1.0f,1.0f);
			glTexCoord2f(0.0,1.0); 
			glVertex3f(-0.5, 1.0f,0.0f); 

			glNormal3f(-1.0f,-1.0f,1.0f);
			glTexCoord2f(0.0,0.0); 
			glVertex3f(-0.5, 0.0f,0.0f);
		glEnd();

		if(!IsRenderShadow)glDisable(GL_ALPHA_TEST);
	glPopMatrix();

	CTree::TotalQuad ++;
	CTree::TotalVertex += 4;
	CTree::TotalNode ++;
	CTree::TotalMemory += sizeof(CLeaf);
}




