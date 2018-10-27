//////////////////////////////////////////////////////////////////////////////////////////
//	SH_TORUS_DoesRayHitObject.cpp
//	Does a given ray intersect the torus?
//	Downloaded from: www.paulsprojects.net
//	Created:	2nd October 2003
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#include <vector>
#include "../../GL files/glee.h"
#include "../../Maths/Maths.h"
#include "../../Log/LOG.h"
#include "../../Bounding Volumes/Bounding Volumes.h"
#include "../../Ray/RAY.h"
#include "../SH_SCENE.h"

bool SH_TORUS::DoesRayHitObject(const RAY & ray)
{
	//First test against the bounding box, if no intersection, return false
	if(!ray.IntersectsAABB(boundingBox, NULL))
		return false;

	//If there is an intersection with the bounding box, test each triangle individually
	for(int i=0; i<(int)(indices.size()/3); ++i)
	{
		//Get the 3 vertex positions for this triangle
		VECTOR3D v[3];
		v[0]=vertices[indices[i*3  ]].position;
		v[1]=vertices[indices[i*3+1]].position;
		v[2]=vertices[indices[i*3+2]].position;

		//If the ray intersects this triangle, return true
		if(ray.IntersectsTriangle(planeEquations[i], v, NULL))
			return true;
	}

	return false;
}