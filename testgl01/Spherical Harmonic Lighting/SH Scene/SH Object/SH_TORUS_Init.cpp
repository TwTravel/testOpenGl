//////////////////////////////////////////////////////////////////////////////////////////
//	SH_TORUS_Init.cpp
//	Initialise a torus
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

bool SH_TORUS::Init(VECTOR3D centre, float innerRadius, float outerRadius, GLuint numSlices,
					GLuint numStacks, COLOR diffuseMaterial)
{
	//Calculate the 2 radii to be used for calculation
	float r1=(outerRadius+innerRadius)/2;				//Centre to centre of tracing circle
	float r2=(outerRadius-innerRadius)/2;				//Radius of tracing circle

	//Fill in the bounding box data
	VECTOR3D mins=centre-VECTOR3D(outerRadius, outerRadius, r2);
	VECTOR3D maxes=centre+VECTOR3D(outerRadius, outerRadius, r2);

	boundingBox.SetFromMinsMaxes(mins, maxes);



	//Calculate the position of the centre of each ring of vertices
	VECTOR3D * ringCentres=new VECTOR3D[numStacks];
	if(!ringCentres)
	{
		LOG::Instance()->OutputError("Unable to create space for %d VECTOR3Ds", numStacks);
		return false;
	}

	for(GLuint i=0; i<numStacks; ++i)
	{
		ringCentres[i]=VECTOR3D(r1, 0.0f, 0.0f).GetRotatedZ(i*360.0f/numStacks);
	}

	//Loop through the rings
	for(GLuint i=0; i<numStacks; ++i)
	{
		//Get the vector from the centre of this ring to the centre of the next
		VECTOR3D tangent;

		if(i==numStacks-1)
			tangent=ringCentres[0]-ringCentres[i];
		else
			tangent=ringCentres[i+1]-ringCentres[i];

		//Loop through the vertices making up this ring
		for(GLuint j=0; j<numSlices; ++j)
		{
			SH_VERTEX currentVertex;

			//Rotate (0, 0, r2) about the tangent vector to form the ring
			currentVertex.position=ringCentres[i]+
						VECTOR3D(0.0f, 0.0f, r2).GetRotatedAxis(j*360.0f/numSlices, tangent);

			currentVertex.position+=centre;

			currentVertex.normal=VECTOR3D(0.0f, 0.0f, 1.0f).
										GetRotatedAxis(j*360.0f/numSlices, tangent);
			
			currentVertex.diffuseMaterial=diffuseMaterial;


			//Add this vertex to the vector
			vertices.push_back(currentVertex);
		}
	}


	//Calculate the indices
	for(GLuint i=0; i<numStacks; ++i)
	{
		for(GLuint j=0; j<numSlices; ++j)
		{
			//Get the index for the 4 vertices around this "quad"
			GLuint quadIndices[4];

			quadIndices[0]=i*numSlices+j;

			if(j!=numSlices-1)
				quadIndices[1]=i*numSlices+j+1;
			else
				quadIndices[1]=i*numSlices;

			if(i!=numStacks-1)
				quadIndices[2]=(i+1)*numSlices+j;
			else
				quadIndices[2]=j;

			if(i!=numStacks-1)
			{
				if(j!=numSlices-1)
					quadIndices[3]=(i+1)*numSlices+j+1;
				else
					quadIndices[3]=(i+1)*numSlices;
			}
			else
			{
				if(j!=numSlices-1)
					quadIndices[3]=j+1;
				else
					quadIndices[3]=0;
			}


			indices.push_back(quadIndices[0]);
			indices.push_back(quadIndices[1]);
			indices.push_back(quadIndices[2]);

			indices.push_back(quadIndices[3]);
			indices.push_back(quadIndices[2]);
			indices.push_back(quadIndices[1]);
		}
	}

	//Delete the ringCentres array
	if(ringCentres)
		delete [] ringCentres;
	ringCentres=NULL;

	//Calculate the plane equation for each triangle
	for(std::size_t i=0; i<indices.size()/3; ++i)
	{
		//Get the 3 vertex positions for this triangle
		VECTOR3D v[3];
		v[0]=vertices[indices[i*3  ]].position;
		v[1]=vertices[indices[i*3+1]].position;
		v[2]=vertices[indices[i*3+2]].position;

		PLANE planeEquation;
		planeEquation.SetFromPoints(v[0], v[1], v[2]);

		planeEquations.push_back(planeEquation);
	}

	return true;
}
