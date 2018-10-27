//////////////////////////////////////////////////////////////////////////////////////////
//	RAY_IntersectsAABB.cpp
//	Does a ray intersect an axis-aligned bounding box?
//	Downloaded from: www.paulsprojects.net
//	Created:	20th September 2003
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#include "../Maths/Maths.h"
#include "../Bounding Volumes/Bounding Volumes.h"
#include "RAY.h"

bool RAY::IntersectsAABB(const AA_BOUNDING_BOX & aabb, float * entryT) const
{
	//If the source of the ray is within the box, there must be an intersection
	if(aabb.IsPointInside(source))
	{
		if(entryT)
			*entryT=0.0f;

		return true;
	}

	//Keep track of the largest tNear and smallest tFar
	float tNear=-99999.0f, tFar=99999.0f;

	//If the ray is parallel to the x planes
	if(direction.x==0.0f)
	{
		//If the ray origin is not between the x extents of the box, return false
		if(source.x<aabb.mins.x || source.x>aabb.maxes.x)
			return false;
	}
	else
	{
		//The ray is not parallel to the planes
		//Compute the intersection distances
		float t1=(aabb.mins.x-source.x)/direction.x;
		float t2=(aabb.maxes.x-source.x)/direction.x;

		//Make sure t1 is the distance to the intersection with the near plane
		if(t1>t2)
		{
			float temp=t1;
			t1=t2;
			t2=temp;
		}

		//Update tNear and tFar
		if(t1>tNear)
			tNear=t1;

		if(t2<tFar)
			tFar=t2;
	}

	//Repeat for Y and Z planes
	if(direction.y==0.0f)
	{
		if(source.y<aabb.mins.y || source.y>aabb.maxes.y)
			return false;
	}
	else
	{
		float t1=(aabb.mins.y-source.y)/direction.y;
		float t2=(aabb.maxes.y-source.y)/direction.y;

		if(t1>t2)
		{
			float temp=t1;
			t1=t2;
			t2=temp;
		}

		if(t1>tNear)
			tNear=t1;

		if(t2<tFar)
			tFar=t2;
	}

	if(direction.z==0.0f)
	{
		if(source.z<aabb.mins.z || source.z>aabb.maxes.z)
			return false;
	}
	else
	{
		float t1=(aabb.mins.z-source.z)/direction.z;
		float t2=(aabb.maxes.z-source.z)/direction.z;

		if(t1>t2)
		{
			float temp=t1;
			t1=t2;
			t2=temp;
		}

		if(t1>tNear)
			tNear=t1;

		if(t2<tFar)
			tFar=t2;
	}

	//If tNear>tFar, no intersection
	if(tNear>tFar)
		return false;

	//If tFar<0, box is behind ray
	if(tFar<0)
		return false;

	//Save entryT if required
	if(entryT)
		*entryT=tNear;

	return true;
}
