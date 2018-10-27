//////////////////////////////////////////////////////////////////////////////////////////
//	RAY_IntersectsTriangle.cpp
//	Does a ray intersect a given triangle?
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

bool RAY::IntersectsTriangle(	const PLANE & planeEquation,
								const VECTOR3D * vertices,
								float * intersectionT) const
{
	//Check the ray and plane are not parallel
	float dot=direction.DotProduct(planeEquation.normal);

	if(dot==0.0f)
		return false;

	//Get the ray parameter where it meets the plane
	float t=-source.DotProduct(planeEquation.normal)-planeEquation.intercept;
	t/=dot;

	//If this parameter is non-positive, no intersection with this triangle
	if(t<=0.0f)
		return false;

	
	//Calculate the point of intersection of ray and plane
	VECTOR3D intersectionPoint=source+t*direction;

	//See if this is inside the triangle
	double angle=0.0;

	//Loop through vertices
	for(int j=0; j<3; ++j)
	{
		//Calculate the vector from this vertex to the intersection point
		const VECTOR3D & vA=vertices[j]-intersectionPoint;

		//Calculate the vector from the next vertex to the point
		const VECTOR3D & vB=vertices[(j+1) % 3]-intersectionPoint;

		//Get the angle between these vectors
		angle+=acos(vA.DotProduct(vB)/(vA.GetLength()*vB.GetLength()));
	}

	//If the sum of the angles is greater than 2*PI, then the point is inside
	if(angle>=1.99*M_PI)
	{
		//Return the intersection parameter if requested
		if(intersectionT)
			*intersectionT=t;

		return true;
	}

	return false;
}
