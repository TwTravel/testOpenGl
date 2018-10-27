//////////////////////////////////////////////////////////////////////////////////////////
//	SH_TORUS.h
//	Class for a torus to be lit by SH lighting. Derives from SH_OBJECT
//	Downloaded from: www.paulsprojects.net
//	Created:	2nd October 2003
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef SH_TORUS_H
#define SH_TORUS_H

class SH_TORUS : public SH_OBJECT
{
public:
	//Initialise the torus
	bool Init(	VECTOR3D centre, float innerRadius, float outerRadius, GLuint numSlices,
				GLuint numStacks, COLOR diffuseMaterial);

	//Does a given ray intersect this object?
	virtual bool DoesRayHitObject(const RAY & ray);

	//Bounding volume
	AA_BOUNDING_BOX boundingBox;

	//Plane equations for each triangle
	std::vector <PLANE> planeEquations;
};

#endif