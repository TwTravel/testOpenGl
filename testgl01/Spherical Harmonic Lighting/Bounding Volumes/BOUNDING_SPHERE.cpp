//////////////////////////////////////////////////////////////////////////////////////////
//	BOUNDING_SPHERE.cpp
//	Functions for bounding sphere. Derives from BOUNDING_VOLUME
//	Downloaded from: www.paulsprojects.net
//	Created:	13th August 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#include <windows.h>
#include "../GL files/glee.h"
#include "../Log/LOG.h"
#include "Bounding Volumes.h"

void BOUNDING_SPHERE::Set(const VECTOR3D & newCentre, const float newRadius)
{
	centre=newCentre;
	radius=newRadius;
}

bool BOUNDING_SPHERE::IsAABoundingBoxInside(const AA_BOUNDING_BOX & box) const
{
	return box.IsBoundingSphereInside((*this));
}

bool BOUNDING_SPHERE::IsPointInside(const VECTOR3D & point) const
{ 
	//get distance from point to centre
	float squaredDistance=(point-centre).GetSquaredLength();

	if(squaredDistance<=radius*radius)
		return true;

	return false;
}

bool BOUNDING_SPHERE::IsBoundingSphereInside(const BOUNDING_SPHERE & sphere) const
{
	//find distance between centres
	float squaredDistance=(sphere.centre-centre).GetSquaredLength();

	//see if this is less than the sum of the radii squared
	if(squaredDistance<=(radius+sphere.radius)*(radius+sphere.radius))
		return true;

	return false;
}

bool BOUNDING_SPHERE::IsPolygonInside(	const PLANE & planeEquation,
										int numVertices,
										const VECTOR3D * vertices) const
{
	//See if the sphere intersects the plane
	//if not, return false
	float distance=planeEquation.GetDistance(centre);

	if(distance>radius || distance<-radius)
		return false;

	//Project the sphere's centre onto the plane of the polygon
	VECTOR3D projectedCentre=centre-distance*planeEquation.normal;


	//See if this is inside the polygon
	double angle=0.0;

	//Loop through the vertices
	for(int i=0; i<numVertices; ++i)
	{
		//Calculate the vector from this vertex to the point
		const VECTOR3D & vA=vertices[i]-projectedCentre;

		//Calculate the vector from the next vertex to the point
		const VECTOR3D & vB=vertices[(i+1) % numVertices]-projectedCentre;

		//Get the angle between these vectors
		angle+=acos(vA.DotProduct(vB)/(vA.GetLength() * vB.GetLength()));
	}

	//If the sum of the angles is greater than 2*Pi (with an error factor) then the point is inside
	if(angle>=1.99*M_PI)
		return true;


	//Check if the sphere intersects any of the edges
	
	//Loop through the edges
	for(int i=0; i<numVertices; ++i)
	{
		//Get the closest point on the current edge to the centre of the sphere
		VECTOR3D closestPoint;

		//Get the 2 vertices on this edge
		const VECTOR3D & v1=vertices[i];
		const VECTOR3D & v2=vertices[(i+1) % numVertices];

		//Get the length of this edge, and a vector along it
		float edgeLength=(v2-v1).GetLength();
		VECTOR3D edgeDirection=v2-v1;
		edgeDirection.Normalize();

		//Get the vector from the first vertex to the centre
		VECTOR3D vertexToCentre=centre-v1;

		//Calculate a parameter for how far along the edge we are
		float t=vertexToCentre.DotProduct(edgeDirection);

		//If t<=0, closestPoint is v1
		//If t>=edgeLength, closestPoint is v2
		//Otherwise, lerp between the points
		if(t<=0)
			closestPoint=v1;
		else if(t>=edgeLength)
			closestPoint=v2;
		else
			closestPoint=v1.lerp(v2, t/edgeLength);

		//Calculate if this point is inside the sphere
		if((closestPoint-centre).GetSquaredLength()<radius*radius)
			return true;
	}

	return false;
}
