//////////////////////////////////////////////////////////////////////////////////////////
//	SH_SPHERE.h
//	Class for a sphere to be lit by SH lighting. Derives from SH_OBJECT
//	Downloaded from: www.paulsprojects.net
//	Created:	2nd October 2003
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef SH_SPHERE_H
#define SH_SPHERE_H

class SH_SPHERE : public SH_OBJECT
{
public:
	//Initialise the sphere
	bool Init(	VECTOR3D centre, float radius, GLuint numSlices, GLuint numStacks,
				COLOR diffuseMaterial);

	//Does a given ray intersect this object?
	virtual bool DoesRayHitObject(const RAY & ray)
	{
		return ray.IntersectsSphere(boundingSphere, NULL);
	}

	//Bounding volume
	BOUNDING_SPHERE boundingSphere;
};

#endif