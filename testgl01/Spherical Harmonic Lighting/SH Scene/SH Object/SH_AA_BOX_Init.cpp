//////////////////////////////////////////////////////////////////////////////////////////
//	SH_AA_BOX_Init.cpp
//	Initialise an axis-aligned box
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

bool SH_AA_BOX::Init(	VECTOR3D mins, VECTOR3D maxes, GLuint xSteps, GLuint ySteps,
						GLuint zSteps, COLOR diffuseMaterial)
{
	//Fill in the bounding box data
	boundingBox.SetFromMinsMaxes(mins, maxes);

	//Generate the +Z plane
	for(GLuint x=0; x<xSteps+1; ++x)
	{
		for(GLuint y=0; y<ySteps+1; ++y)
		{
			SH_VERTEX currentVertex;

			currentVertex.position.Set(	mins.x + (float(x)/xSteps)*(maxes.x-mins.x),
										mins.y + (float(y)/ySteps)*(maxes.y-mins.y),
										maxes.z);

			currentVertex.normal.Set(0.0f, 0.0f, 1.0f);
			
			currentVertex.diffuseMaterial=diffuseMaterial;

			vertices.push_back(currentVertex);
		}
	}

	//Generate the indices
	for(GLuint x=0; x<xSteps; ++x)
	{
		for(GLuint y=0; y<ySteps; ++y)
		{
			indices.push_back(x*(ySteps+1)+y);
			indices.push_back((x+1)*(ySteps+1)+y);
			indices.push_back(x*(ySteps+1)+y+1);

			indices.push_back((x+1)*(ySteps+1)+y);
			indices.push_back((x+1)*(ySteps+1)+y+1);
			indices.push_back(x*(ySteps+1)+y+1);
		}
	}


	//Generate the -Z plane
	GLuint indexBase=vertices.size();

	for(GLuint x=0; x<xSteps+1; ++x)
	{
		for(GLuint y=0; y<ySteps+1; ++y)
		{
			SH_VERTEX currentVertex;

			currentVertex.position.Set(	mins.x + (float(x)/xSteps)*(maxes.x-mins.x),
										mins.y + (float(y)/ySteps)*(maxes.y-mins.y),
										mins.z);

			currentVertex.normal.Set(0.0f, 0.0f,-1.0f);
			
			currentVertex.diffuseMaterial=diffuseMaterial;

			vertices.push_back(currentVertex);
		}
	}

	//Generate the indices
	for(GLuint x=0; x<xSteps; ++x)
	{
		for(GLuint y=0; y<ySteps; ++y)
		{
			indices.push_back(indexBase+x*(ySteps+1)+y);
			indices.push_back(indexBase+x*(ySteps+1)+y+1);
			indices.push_back(indexBase+(x+1)*(ySteps+1)+y);

			indices.push_back(indexBase+(x+1)*(ySteps+1)+y);
			indices.push_back(indexBase+x*(ySteps+1)+y+1);
			indices.push_back(indexBase+(x+1)*(ySteps+1)+y+1);
		}
	}


	//Generate the +X plane
	indexBase=vertices.size();

	for(GLuint z=0; z<zSteps+1; ++z)
	{
		for(GLuint y=0; y<ySteps+1; ++y)
		{
			SH_VERTEX currentVertex;

			currentVertex.position.Set(	maxes.x,
										mins.y + (float(y)/ySteps)*(maxes.y-mins.y),
										mins.z + (float(z)/zSteps)*(maxes.z-mins.z));

			currentVertex.normal.Set(1.0f, 0.0f, 0.0f);
			
			currentVertex.diffuseMaterial=diffuseMaterial;

			vertices.push_back(currentVertex);
		}
	}

	//Generate the indices
	for(GLuint z=0; z<zSteps; ++z)
	{
		for(GLuint y=0; y<ySteps; ++y)
		{
			indices.push_back(indexBase+z*(ySteps+1)+y);
			indices.push_back(indexBase+z*(ySteps+1)+y+1);
			indices.push_back(indexBase+(z+1)*(ySteps+1)+y);

			indices.push_back(indexBase+(z+1)*(ySteps+1)+y);
			indices.push_back(indexBase+z*(ySteps+1)+y+1);
			indices.push_back(indexBase+(z+1)*(ySteps+1)+y+1);
		}
	}


	//Generate the -X plane
	indexBase=vertices.size();

	for(GLuint z=0; z<zSteps+1; ++z)
	{
		for(GLuint y=0; y<ySteps+1; ++y)
		{
			SH_VERTEX currentVertex;

			currentVertex.position.Set(	mins.x,
										mins.y + (float(y)/ySteps)*(maxes.y-mins.y),
										mins.z + (float(z)/zSteps)*(maxes.z-mins.z));

			currentVertex.normal.Set(-1.0f, 0.0f, 0.0f);
			
			currentVertex.diffuseMaterial=diffuseMaterial;

			vertices.push_back(currentVertex);
		}
	}

	//Generate the indices
	for(GLuint z=0; z<zSteps; ++z)
	{
		for(GLuint y=0; y<ySteps; ++y)
		{
			indices.push_back(indexBase+z*(ySteps+1)+y);
			indices.push_back(indexBase+(z+1)*(ySteps+1)+y);
			indices.push_back(indexBase+z*(ySteps+1)+y+1);

			indices.push_back(indexBase+(z+1)*(ySteps+1)+y);
			indices.push_back(indexBase+(z+1)*(ySteps+1)+y+1);
			indices.push_back(indexBase+z*(ySteps+1)+y+1);
		}
	}


	//Generate the +Y plane
	indexBase=vertices.size();

	for(GLuint x=0; x<xSteps+1; ++x)
	{
		for(GLuint z=0; z<zSteps+1; ++z)
		{
			SH_VERTEX currentVertex;

			currentVertex.position.Set(	mins.x + (float(x)/xSteps)*(maxes.x-mins.x),
										maxes.y,
										mins.z + (float(z)/zSteps)*(maxes.z-mins.z));

			currentVertex.normal.Set(0.0f, 1.0f, 0.0f);
			
			currentVertex.diffuseMaterial=diffuseMaterial;

			vertices.push_back(currentVertex);
		}
	}

	//Generate the indices
	for(GLuint x=0; x<xSteps; ++x)
	{
		for(GLuint z=0; z<zSteps; ++z)
		{
			indices.push_back(indexBase+x*(zSteps+1)+z);
			indices.push_back(indexBase+x*(zSteps+1)+z+1);
			indices.push_back(indexBase+(x+1)*(zSteps+1)+z);

			indices.push_back(indexBase+(x+1)*(zSteps+1)+z);
			indices.push_back(indexBase+x*(zSteps+1)+z+1);
			indices.push_back(indexBase+(x+1)*(zSteps+1)+z+1);
		}
	}


	//Generate the -Y plane
	indexBase=vertices.size();

	for(GLuint x=0; x<xSteps+1; ++x)
	{
		for(GLuint z=0; z<zSteps+1; ++z)
		{
			SH_VERTEX currentVertex;

			currentVertex.position.Set(	mins.x + (float(x)/xSteps)*(maxes.x-mins.x),
										mins.y,
										mins.z + (float(z)/zSteps)*(maxes.z-mins.z));

			currentVertex.normal.Set(0.0f,-1.0f, 0.0f);
			
			currentVertex.diffuseMaterial=diffuseMaterial;

			vertices.push_back(currentVertex);
		}
	}

	//Generate the indices
	for(GLuint x=0; x<xSteps; ++x)
	{
		for(GLuint z=0; z<zSteps; ++z)
		{
			indices.push_back(indexBase+x*(zSteps+1)+z);
			indices.push_back(indexBase+(x+1)*(zSteps+1)+z);
			indices.push_back(indexBase+x*(zSteps+1)+z+1);

			indices.push_back(indexBase+(x+1)*(zSteps+1)+z);
			indices.push_back(indexBase+(x+1)*(zSteps+1)+z+1);
			indices.push_back(indexBase+x*(zSteps+1)+z+1);
		}
	}	

	return true;
}