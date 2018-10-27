//////////////////////////////////////////////////////////////////////////////////////////
//	RAY.h
//	Class declaration for a ray
//	Downloaded from: www.paulsprojects.net
//	Created:	21st September 2003
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef RAY_H
#define RAY_H

class RAY
{
public:
	RAY()
	{}
	RAY(VECTOR3D newSource, VECTOR3D newDirection)	:	source(newSource)
	{
		direction=newDirection.GetNormalized();
	}

	~RAY()
	{}

	void Set(VECTOR3D newSource, VECTOR3D newDirection)
	{
		source=newSource;
		direction=newDirection.GetNormalized();
	}

	//Does this ray intersect a given triangle? Return the parameter of intersection if requested
	bool IntersectsTriangle(const PLANE & planeEquation,
							const VECTOR3D * vertices,
							float * intersectionT) const;

	//Does the ray intersect an axis-aligned bounding box? Return the parameter of entry
	bool IntersectsAABB(const AA_BOUNDING_BOX & aabb, float * entryT) const;

	//Does the ray intersect a sphere? Return the parameter at entry
	bool IntersectsSphere(const BOUNDING_SPHERE & sphere, float * entryT) const;

	VECTOR3D source;			//Source of ray
	VECTOR3D direction;			//Direction (normalised)
};

#endif