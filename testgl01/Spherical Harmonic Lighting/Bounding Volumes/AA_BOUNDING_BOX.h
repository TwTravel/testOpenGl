//////////////////////////////////////////////////////////////////////////////////////////
//	AA_BOUNDING_BOX.h
//	class declaration for axis aligned bounding box, derives from BOUNDING_VOLUME
//	Downloaded from: www.paulsprojects.net
//	Created:	13th August 2002
//	Modified:	21st November 2002	-	CHanged mins and maxes from float* to VECTOR3D
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef AA_BOUNDING_BOX_H
#define AA_BOUNDING_BOX_H

#include "BOUNDING_VOLUME.h"

//planes of box
enum BOUNDING_BOX_PLANES
{
	BOUNDING_BOX_NEG_X_PLANE=0,
	BOUNDING_BOX_POS_X_PLANE,
	BOUNDING_BOX_NEG_Y_PLANE,
	BOUNDING_BOX_POS_Y_PLANE,
	BOUNDING_BOX_POS_Z_PLANE,
	BOUNDING_BOX_NEG_Z_PLANE
};

class AA_BOUNDING_BOX : public BOUNDING_VOLUME
{
public:
	void SetFromMinsMaxes(const VECTOR3D & newMins, const VECTOR3D & newMaxes);
	
	virtual bool IsPointInside(const VECTOR3D & point) const;
	virtual bool IsAABoundingBoxInside(const AA_BOUNDING_BOX & box) const;
	virtual bool IsBoundingSphereInside(const BOUNDING_SPHERE & sphere) const;

	virtual bool IsPolygonInside(	const PLANE & planeEquation, int numVertices,
									const VECTOR3D * vertices) const;

	//Classify the box by a single plane
	int ClassifyByPlane(const PLANE & plane) const;

	//Draw the bounding box
	void Draw() const;

	VECTOR3D vertices[8];
	PLANE planes[6];
	VECTOR3D mins, maxes;
};

//Results for ClassifyByPlane
const int BOX_STRADDLING_PLANE=0;
const int BOX_IN_FRONT_OF_PLANE=1;
const int BOX_BEHIND_PLANE=2;

#endif	//AA_BOUNDING_BOX_H