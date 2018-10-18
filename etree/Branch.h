//****************************************************************************
//  Branch.h: interface for the CBranch class.
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

#if !defined(AFX_BRANCH_H__B2340C7E_7701_4EBC_95B1_5E9A7BADDDD6__INCLUDED_)
#define AFX_BRANCH_H__B2340C7E_7701_4EBC_95B1_5E9A7BADDDD6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTree;

#include <math.h>
#include "ChainList.h"
#include "GL_helper.h"

#define SEG_END_LEAF_COUNT 2

#include "Leaf.h"

#define TriZoom(x) ((x)*0.8660254f)

class CSegment;

class CBranch
{
public:
	CBranch();
	virtual ~CBranch();

	void Render();
	bool Rebuild(int);
	void FreeAll();

	friend class CTree;
	friend class CSegment;

protected:
	CSegment *	Segments;
	CSegment *	Base;

	GLfloat	 LeafScale;
};

class CSegment:public CChainNode<CSegment>
{
public:
	CSegment();
	~CSegment();
	void BuildChildren(int Count,GLfloat LeafSize);
	void Render(CSegment & Father);

	friend class CTree;
	friend class CBranch;

	inline void SetTwist(float ex)   //in Degree
		{double tmp=(double)Deg2Rad(Twist = ex); SinTwist = sin(tmp); CosTwist = cos(tmp);}
	inline void SetExpand(float ex)  //in Degree
		{double tmp=(double)Deg2Rad(Expand = ex); SinExpand = sin(tmp); CosExpand = cos(tmp);}
	inline void SetLength(float Len){/*ASSERT(Len>0.000001f);*/ Length = Len; }
	inline void SetRadius(float rd){/*ASSERT(rd>0.000001f);*/ TriRadius = TriZoom(Radius = rd); HalfRadius = Radius*0.5f; }

	/////////////
	//Param
	static float SegExpand,SegScaleLen,SegScaleRad,SegLeafSize,LeafExpand;
	static float SegTwistInc,SegSideExpand,SegSideScaleDepth,SegSideScale;
	static CglMaterial BarkMTR;

protected:
	/// Twist
	double SinTwist,CosTwist,Twist;
	/// Expand
	double SinExpand,CosExpand,Expand;
	/// Length
	GLfloat Length;
	/// Radius
	GLfloat Radius;
	GLfloat HalfRadius; //HalfRadius = 0.5*Radius
	GLfloat TriRadius;  //TriRadius = Radius*((3)^(0.5)/2) = TriZoom(...)

	static CglTexture Tex;

	/////////////////////////////////////////
	//SideBranch
	CBranch * SideBranch;
	CLeaf * Leaves;

	//////////////////////////
	void DefineOneLine(GLfloat x,GLfloat z,GLfloat fx,GLfloat fz,GLfloat Tex);
};


#endif // !defined(AFX_BRANCH_H__B2340C7E_7701_4EBC_95B1_5E9A7BADDDD6__INCLUDED_)
