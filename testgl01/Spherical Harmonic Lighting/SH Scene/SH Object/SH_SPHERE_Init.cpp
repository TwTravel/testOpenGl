//////////////////////////////////////////////////////////////////////////////////////////
//	SH_SPHERE_Init.cpp
//	Initialise a sphere
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

bool SH_SPHERE::Init(	VECTOR3D centre, float radius, GLuint numSlices, GLuint numStacks,
						COLOR diffuseMaterial)
{
	//Fill in the bounding sphere data
	boundingSphere.Set(centre, radius);



	//Generate the sphere vertices

	//First vertex at the top
	SH_VERTEX sphereTop;
	sphereTop.position.Set(0.0f, 0.0f, radius);
	sphereTop.position+=centre;
	sphereTop.normal.Set(0.0f, 0.0f, 1.0f);
	sphereTop.diffuseMaterial=diffuseMaterial;
	vertices.push_back(sphereTop);

	//Loop through stacks
	for(GLuint stack=1; stack<numStacks; ++stack)
	{
		double theta=M_PI*(double(stack)/numStacks);

		for(GLuint slice=0; slice<numSlices; ++slice)
		{
			double phi=2*M_PI*(double(slice)/numSlices);

			SH_VERTEX currentVertex;

			currentVertex.position.Set(	float(radius*sin(theta)*cos(phi)),
										float(radius*sin(theta)*sin(phi)),
										float(radius*cos(theta)));

			currentVertex.normal=currentVertex.position.GetNormalized();

			currentVertex.position+=centre;

			currentVertex.diffuseMaterial=diffuseMaterial;

			vertices.push_back(currentVertex);
		}
	}

	//Finally add a vertex at the bottom
	SH_VERTEX sphereBottom;
	sphereBottom.position.Set(0.0f, 0.0f, -radius);
	sphereBottom.position+=centre;
	sphereBottom.normal.Set(0.0f, 0.0f,-1.0f);
	sphereBottom.diffuseMaterial=diffuseMaterial;
	vertices.push_back(sphereBottom);



	//Generate the sphere indices

	//The first stack is a triangle fan
	for(GLuint slice=0; slice<numSlices; ++slice)
	{
		indices.push_back(0);
		indices.push_back(1+slice);
		indices.push_back(1+(slice==numSlices-1 ? 0 : slice+1));
	}

	//Now do all but the last stack
	for(GLuint stack=1; stack<numStacks-1; ++stack)
	{
		for(GLuint slice=0; slice<numSlices; ++slice)
		{
			indices.push_back(1+(stack-1)*numSlices+slice);
			indices.push_back(1+stack*numSlices+slice);
			indices.push_back(1+(stack-1)*numSlices+(slice==numSlices-1 ? 0 : slice+1));

			indices.push_back(1+stack*numSlices+slice);
			indices.push_back(1+stack*numSlices+(slice==numSlices-1 ? 0 : slice+1));
			indices.push_back(1+(stack-1)*numSlices+(slice==numSlices-1 ? 0 : slice+1));
		}
	}

	//The final stack is a triangle fan
	for(GLuint slice=0; slice<numSlices; ++slice)
	{
		indices.push_back(1+(numStacks-2)*numSlices+slice);
		indices.push_back(1+(numStacks-1)*numSlices);
		indices.push_back(1+(numStacks-2)*numSlices+(slice==numSlices-1 ? 0 : slice+1));
	}

	return true;
}
