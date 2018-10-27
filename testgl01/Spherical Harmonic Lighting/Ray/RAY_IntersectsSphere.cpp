//////////////////////////////////////////////////////////////////////////////////////////
//	RAY_IntersectsSphere.cpp
//	Does a ray intersect a sphere?
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

bool RAY::IntersectsSphere(const BOUNDING_SPHERE & sphere, float * entryT) const
{
	//If the source of the ray is within the sphere, there must be an intersection
	if(sphere.IsPointInside(source))
	{
		if(entryT)
			*entryT=0.0f;

		return true;
	}

	//Otherwise, solve a quadratic to find the intersection. a==1
	float b=2.0f*(	direction.x*(source.x-sphere.centre.x) +
					direction.y*(source.y-sphere.centre.y) +
					direction.z*(source.z-sphere.centre.z));

	float c=	(source.x-sphere.centre.x)*(source.x-sphere.centre.x) +
				(source.y-sphere.centre.y)*(source.y-sphere.centre.y) +
				(source.z-sphere.centre.z)*(source.z-sphere.centre.z) -
				sphere.radius*sphere.radius;

	//If the discriminant is negative, no intersection
	float d=b*b-4*c;
	
	if(d<0)
		return false;

	//Otherwise, the smaller positive root is the intersection parameter
	float r1=(-b+sqrtf(d))/2;
	float r2=(-b-sqrtf(d))/2;

	//If both roots are positive, return true, entryT is the smaller
	if(r1>0 && r2>0)
	{
		if(entryT)
			*entryT=(r1<r2 ? r1 : r2);

		return true;
	}

	//If both roots are negative, no intersection
	if(r1<0 && r2<0)
		return false;

	//Otherwise, one is negative and one is positive. Return true, entryT is the positive one
	if(entryT)
		*entryT=(r1<r2 ? r2 : r1);

	return true;
}
