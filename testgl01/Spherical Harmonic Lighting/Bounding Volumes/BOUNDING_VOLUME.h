//////////////////////////////////////////////////////////////////////////////////////////
//	BOUNDING_VOLUME.h
//	class declaration for an abstract base class for bounding volumes
//	Downloaded from: www.paulsprojects.net
//	Created:	13th August 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef BOUNDING_VOLUME_H
#define BOUNDING_VOLUME_H

//forward declarations
class AA_BOUNDING_BOX;
class BOUNDING_SPHERE;

class BOUNDING_VOLUME
{
public:
	virtual bool IsPointInside(const VECTOR3D & point) const =0;
	virtual bool IsAABoundingBoxInside(const AA_BOUNDING_BOX & box)const =0;
	virtual bool IsBoundingSphereInside(const BOUNDING_SPHERE & sphere)const =0;

	virtual bool IsPolygonInside(	const PLANE & planeEquation, int numVertices,
									const VECTOR3D * vertices) const =0;
};

#endif	//BOUNDING_VOLUME_H