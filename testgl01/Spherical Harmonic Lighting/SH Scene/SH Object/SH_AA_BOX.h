//////////////////////////////////////////////////////////////////////////////////////////
//	SH_AA_BOX.h
//	Class for an axis-aligned box to be lit by SH lighting. Derives from SH_OBJECT
//	Downloaded from: www.paulsprojects.net
//	Created:	2nd October 2003
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef SH_AA_BOX_H
#define SH_AA_BOX_H

class SH_AA_BOX : public SH_OBJECT
{
public:
	//Initialise the box
	bool Init(	VECTOR3D mins, VECTOR3D maxes, GLuint xSteps, GLuint ySteps, GLuint zSteps,
				COLOR diffuseMaterial);

	//Does a given ray intersect this object?
	virtual bool DoesRayHitObject(const RAY & ray)
	{
		return ray.IntersectsAABB(boundingBox, NULL);
	}

	//Bounding volume
	AA_BOUNDING_BOX boundingBox;
};

#endif