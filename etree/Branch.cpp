//****************************************************************************
//  Branch.cpp: implementation of the CBranch class.
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
#include "Branch.h"
#include "tree.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////
//Implement for CSegment
////////////////////////////////////////////////////
CglMaterial CSegment::BarkMTR;

float CSegment::SegLeafSize	= 1.970f;

float CSegment::LeafExpand	= 26.4f;

float CSegment::SegExpand	= 26.4f;
float CSegment::SegScaleLen = 0.83f;
float CSegment::SegScaleRad = 0.75f;
float CSegment::SegTwistInc = 61.02f;
float CSegment::SegSideExpand = 58.8f;
float CSegment::SegSideScaleDepth = 0.81f;
float CSegment::SegSideScale = 0.93f;

CglTexture CSegment::Tex(_T("bark.bmp"),GL_Tex_BMP,7);

#define SegDefaultLength 8.0f

CSegment::~CSegment()
{
	if(SideBranch)
	{
		delete SideBranch;
		SideBranch = NULL;
	}

	if(Leaves)
	{
		delete Leaves;
		Leaves = NULL;
	}
}


CSegment::CSegment()
{
	SetExpand(0.0f);
	SetLength(SegDefaultLength);
	SetRadius(0.8f);
	SetTwist(0.0f);

	SideBranch = NULL;
	Leaves = NULL;
}

void CSegment::Render(CSegment &Father)
{
	Tex.Apply();
	BarkMTR.Apply();
	glColor4f(0,0,0,CTree::BranchShadowDensity);

	glBegin(GL_QUAD_STRIP);
		DefineOneLine(HalfRadius,	TriRadius,	Father.HalfRadius,	Father.TriRadius,	0.0f			);    // 0/6
		DefineOneLine(-HalfRadius,	TriRadius,	-Father.HalfRadius,	Father.TriRadius,	0.1666666667f	); // 1/6
		DefineOneLine(-Radius,		0.0f,		-Father.Radius,		0.0f,				0.3333333333f	); // 2/6  and so on
		DefineOneLine(-HalfRadius,	-TriRadius,	-Father.HalfRadius,	-Father.TriRadius,	0.5f			);
		DefineOneLine(HalfRadius,	-TriRadius,	Father.HalfRadius,	-Father.TriRadius,	0.6666666667f	);
		DefineOneLine(Radius,		0.0f,		Father.Radius,		0.0f,				0.8333333333f	);
		DefineOneLine(HalfRadius,	TriRadius,	Father.HalfRadius,	Father.TriRadius,	1.0f			);    // 6/6
	glEnd();

	CTree::TotalQuad += 6;
	CTree::TotalVertex += 12;
	CTree::TotalNode ++;
	CTree::TotalMemory += sizeof(CSegment);
}

void CSegment::DefineOneLine(GLfloat x, GLfloat z, GLfloat fx, GLfloat fz, GLfloat Tex)
{
	glNormal3f(fx,0.0f,fz); 
	glTexCoord2f(Tex,0.0f); 
	glVertex3f(fx, 0.0f, fz); 

	{GLfloat x3,y3,z3,x2,z2,x1;

		x1 = - z*SinTwist + x*CosTwist;
		z2 = z*CosTwist + x*SinTwist;
		x2 = x1*CosExpand - Length*SinExpand;
		z3 = z2*CosTwist - x2*SinTwist;
		y3 = x1*SinExpand + Length*CosExpand;
		x3 = z2*SinTwist + x2*CosTwist;
		/**************************************************************
		The codes above is the simulate the following GL calls which will be ignored within glBegin/glEnd
		glPushMatrix(); 
		glTranslatef(0.0f,Length,0.0f); 
		glRotatef(Expand, CosTwist, 0.0f, SinTwist); 
		glPopMatrix(); 
		***************************************************************/

		glNormal3f(fx,0.0f,fz);  //glNormal3f(x3,y3,z3);  
		glTexCoord2f(Tex,1.0f); 
		glVertex3f(x3,y3,z3); 
	} 
}

void CSegment::BuildChildren(int Count,GLfloat LeafSize)
{
	if(Count)
	{
		ASSERT(!Next);
		ASSERT(!SideBranch);
		Next = new CSegment;
		ASSERT(Next);
		if(Next)  //step into
		{
			Next->SetExpand(SegExpand);  
			Next->SetLength(SegScaleLen*Length);
			Next->SetRadius(SegScaleRad*Radius);
			Next->SetTwist(SegTwistInc+Twist);

			Next->BuildChildren(Count - 1,LeafSize);
		}
	}

	if(Count>2)
	{//  Build sidebranch
		SideBranch = new CBranch;
		ASSERT(SideBranch); 
		if(SideBranch)
		{
			SideBranch->LeafScale = LeafSize/CSegment::SegSideScale;
			SideBranch->Base = this;
			SideBranch->Rebuild(max(1.0f,min(Count,Count*SegSideScaleDepth)));
		}
	}
	else  
	{//  Build leaves
		Leaves = new CLeaf;
		Leaves[0].Expand = LeafExpand;
		Leaves[0].Scale = LeafSize;
		Leaves[0].Twist = SegTwistInc+Twist + 180;
	}
}


//////////////////////////////////////////////////////////////////////
// Implement for CBranch
//////////////////////////////////////////////////////////////////////

CBranch::CBranch()
{
	Base = Segments = NULL;
	LeafScale = 0.0;
}

CBranch::~CBranch()
{
	FreeAll();
}

bool CBranch::Rebuild(int Count)
{
	ASSERT(Count>=1);
	if(Segments)delete Segments;
	Segments = new CSegment;
	ASSERT(Segments);
	if(Base)
	{
		Segments->SetLength(CSegment::SegScaleLen*Base->Length);
		Segments->SetRadius(CSegment::SegScaleRad*Base->Radius);
		if(Base->Next)
		{
			Segments->SetExpand(fabs(CSegment::SegSideExpand - Base->Next->Expand));
			Segments->SetTwist(Base->Next->Twist+180.0f);
		}
	}
	if(Segments)
	{
		Segments->BuildChildren(Count - 1,LeafScale);
	}
	else
	{
		return false;
	}

	return true;
}

void CBranch::Render()
{
	glPushMatrix();

		ASSERT(Segments);
		CSegment *seg,*ne;
		seg = Segments;

		if(Base)
		{
			Segments->Render(*Base);
			glRotatef(Segments->Expand, Segments->SinTwist, 0.0f,Segments->CosTwist); 
			glTranslatef(0.0f,Segments->Length,0.0f); 
		}

		while(ne = seg->GetNext())
		{
			if(seg->SideBranch)
			{
				glPushMatrix();
					glScalef(CSegment::SegSideScale,CSegment::SegSideScale,CSegment::SegSideScale);
					seg->SideBranch->Render();
				glPopMatrix();
			}

			if(seg->Leaves)
			{
				seg->Leaves[0].Render();
			}

			ne->Render(*seg);
			glRotatef(ne->Expand, ne->SinTwist, 0.0f,ne->CosTwist); 
			glTranslatef(0.0f,ne->Length,0.0f); 
			seg = ne;
		}

		if(seg->Leaves)
		{
			seg->Leaves[0].Render();
		}

	glPopMatrix();

	CTree::TotalNode ++;
	CTree::TotalMemory += sizeof(CBranch);
}


void CBranch::FreeAll()
{
	if(Segments)delete Segments;
	Segments = NULL;
}
