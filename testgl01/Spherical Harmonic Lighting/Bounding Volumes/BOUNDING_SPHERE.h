//////////////////////////////////////////////////////////////////////////////////////////
//	BOUNDING_SPHERE.h
//	class declaration for bounding sphere, derives from BOUNDING_VOLUME
//	Downloaded from: www.paulsprojects.net
//	Created:	13th August 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef BOUNDING_SPHERE_H
#define BOUNDING_SPHERE_H

class BOUNDING_SPHERE : public BOUNDING_VOLUME
{
public:
	void Set(const VECTOR3D & newCentre, const float newRadius);
	
	virtual bool IsPointInside(const VECTOR3D & point) const;
	virtual bool IsAABoundingBoxInside(const AA_BOUNDING_BOX & box) const;
	virtual bool IsBoundingSphereInside(const BOUNDING_SPHERE & sphere) const;

	virtual bool IsPolygonInside(	const PLANE & planeEquation, int numVertices,
									const VECTOR3D * vertices) const;

	VECTOR3D centre;
	float radius;
};

#endif	//BOUNDING_SPHERE_H