//////////////////////////////////////////////////////////////////////////////////////////
//	AA_BOUNDING_BOX.cpp
//	Functions for axis aligned bounding box. Derives from BOUNDING_VOLUME
//	Downloaded from: www.paulsprojects.net
//	Created:	13th August 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#include <windows.h>
#include "../GL files/glee.h"
#include "../Log/LOG.h"
#include "Bounding Volumes.h"

void AA_BOUNDING_BOX::SetFromMinsMaxes(const VECTOR3D & newMins, const VECTOR3D & newMaxes)
{
	//Save mins & maxes
	mins=newMins;
	maxes=newMaxes;

	//set the positions of the vertices
	vertices[0].Set(mins.x, mins.y, mins.z);
	vertices[1].Set(mins.x, mins.y, maxes.z);
	vertices[2].Set(mins.x, maxes.y, mins.z);
	vertices[3].Set(mins.x, maxes.y, maxes.z);
	vertices[4].Set(maxes.x, mins.y, mins.z);
	vertices[5].Set(maxes.x, mins.y, maxes.z);
	vertices[6].Set(maxes.x, maxes.y, mins.z);
	vertices[7].Set(maxes.x, maxes.y, maxes.z);

	//set up the planes
	planes[BOUNDING_BOX_NEG_X_PLANE].normal.Set(1.0f, 0.0f, 0.0f);
	planes[BOUNDING_BOX_NEG_X_PLANE].intercept=-mins.x;

	planes[BOUNDING_BOX_POS_X_PLANE].normal.Set(-1.0f, 0.0f, 0.0f);
	planes[BOUNDING_BOX_POS_X_PLANE].intercept=maxes.x;

	planes[BOUNDING_BOX_NEG_Y_PLANE].normal.Set(0.0f, 1.0f, 0.0f);
	planes[BOUNDING_BOX_NEG_Y_PLANE].intercept=-mins.y;

	planes[BOUNDING_BOX_POS_Y_PLANE].normal.Set(0.0f,-1.0f, 0.0f);
	planes[BOUNDING_BOX_POS_Y_PLANE].intercept=maxes.y;

	planes[BOUNDING_BOX_NEG_Z_PLANE].normal.Set(0.0f, 0.0f, 1.0f);
	planes[BOUNDING_BOX_NEG_Z_PLANE].intercept=-mins.z;

	planes[BOUNDING_BOX_POS_Z_PLANE].normal.Set(0.0f, 0.0f,-1.0f);
	planes[BOUNDING_BOX_POS_Z_PLANE].intercept=maxes.z;
}



//is a point in the box
bool AA_BOUNDING_BOX::IsPointInside(const VECTOR3D & point) const
{
	for(int i=0; i<6; ++i)
	{
		if(planes[i].ClassifyPoint(point)==POINT_BEHIND_PLANE)
			return false;
	}

	return true;
}

//do two AABBs intersect?
bool AA_BOUNDING_BOX::IsAABoundingBoxInside(const AA_BOUNDING_BOX & box) const
{
	//loop through vertices of the other box
	for(int i=0; i<8; ++i)
	{
		//if this vertex is in this box, return true
		if(IsPointInside(box.vertices[i]))
			return true;
	}

	//now see if any of our vertices are in the other box
	for(int i=0; i<8; ++i)
	{
		//if this vertex is in this box, return true
		if(box.IsPointInside(vertices[i]))
			return true;
	}

	return false;
}

bool AA_BOUNDING_BOX::IsBoundingSphereInside(const BOUNDING_SPHERE & sphere) const
{
	for(int i=0; i<6; ++i)
	{
		if(	planes[i].normal.x*sphere.centre.x +
			planes[i].normal.y*sphere.centre.y +
			planes[i].normal.z*sphere.centre.z +
			planes[i].intercept < -sphere.radius)
			return false;
	}

	return true;
}

bool AA_BOUNDING_BOX::IsPolygonInside(	const PLANE & planeEquation, int numVertices,
										const VECTOR3D * vertices) const
{
	//loop through planes
	for(int i=0; i<6; ++i)
	{
		//If all vertices are behind this plane, return false
		bool allBehind=true;
		
		for(int j=0; j<numVertices; ++j)
		{
			if(planes[i].ClassifyPoint(vertices[j])!=POINT_BEHIND_PLANE)
			{
				allBehind=false;
				break;
			}
		}

		if(allBehind)
			return false;
	}

	return true;
}

//Classify by a single plane
int AA_BOUNDING_BOX::ClassifyByPlane(const PLANE & plane) const
{
	bool vertexInFront=false;
	bool vertexBehind=false;
	bool vertexOnPlane=false;

	//Loop through vertices
	for(int i=0; i<8; ++i)
	{
		int vertexClassification=plane.ClassifyPoint(vertices[i]);

		if(vertexClassification==POINT_IN_FRONT_OF_PLANE)
			vertexInFront=true;

		if(vertexClassification==POINT_ON_PLANE)
			vertexOnPlane=true;

		if(vertexClassification==POINT_BEHIND_PLANE)
			vertexBehind=true;
	}

	if(vertexInFront==true && vertexBehind==false && vertexOnPlane==false)
		return BOX_IN_FRONT_OF_PLANE;

	if(vertexBehind==true && vertexInFront==false && vertexOnPlane==false)
		return BOX_BEHIND_PLANE;

	return BOX_STRADDLING_PLANE;
}

//Draw the bounding box
void AA_BOUNDING_BOX::Draw() const
{
	glBegin(GL_LINE_LOOP);
	{
		glVertex3fv(vertices[0]);
		glVertex3fv(vertices[1]);
		glVertex3fv(vertices[3]);
		glVertex3fv(vertices[2]);
	}
	glEnd();

	glBegin(GL_LINE_LOOP);
	{
		glVertex3fv(vertices[4]);
		glVertex3fv(vertices[5]);
		glVertex3fv(vertices[7]);
		glVertex3fv(vertices[6]);
	}
	glEnd();

	glBegin(GL_LINES);
	{
		glVertex3fv(vertices[0]);
		glVertex3fv(vertices[4]);
		glVertex3fv(vertices[1]);
		glVertex3fv(vertices[5]);
		glVertex3fv(vertices[3]);
		glVertex3fv(vertices[7]);
		glVertex3fv(vertices[2]);
		glVertex3fv(vertices[6]);
	}
	glEnd();
}