//////////////////////////////////////////////////////////////////////////////////////////
//	SH_SCENE_GenerateDirectCoeffs.cpp
//	Generate the SH coefficients for the direct lighting transfer functions
//	Downloaded from: www.paulsprojects.net
//	Created:	2nd October 2003
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#include <vector>
#include <fstream>
#include "../GL files/glee.h"
#include "../Maths/Maths.h"
#include "../Log/LOG.h"
#include "../Window/WINDOW.h"
#include "../Bitmap Font/BITMAP_FONT.h"
#include "../Bounding Volumes/Bounding Volumes.h"
#include "../Ray/RAY.h"
#include "SH_SCENE.h"

extern BITMAP_FONT font;

bool SH_SCENE::GenerateDirectCoeffs(int numBands, int numSamples, SAMPLE * samples)
{
	const int numFunctions=numBands*numBands;
	const int numObjects=objects.size();

	//Create space for the SH coefficients in each vertex
	for(int i=0; i<numObjects; ++i)
	{
		SH_OBJECT * currentObject=objects[i];
		const int numVertices=currentObject->vertices.size();

		for(int j=0; j<numVertices; ++j)
		{
			SH_VERTEX & currentVertex=currentObject->vertices[j];

			currentVertex.unshadowedCoeffs=new double[numFunctions];
			currentVertex.shadowedCoeffs=new double[numFunctions];

			if(!currentVertex.unshadowedCoeffs || !currentVertex.shadowedCoeffs)
			{
				LOG::Instance()->OutputError("Unable to create space for vertex SH coefficients");
				return false;
			}
		}
	}

	//Is there a file containing the coefficients, or do they need to be regenerated?
	bool regenerateCoeffs=false;

	//Does the file exist?
	std::ifstream inFile("directcoeffs.dat", std::ios::in | std::ios::binary);
	if(!inFile.is_open())
	{
		LOG::Instance()->OutputMisc("Unable to open directcoeffs.dat, regenerating coefficients...");
		regenerateCoeffs=true;
	}

	//Are the number of bands and samples in the file correct?
	if(!regenerateCoeffs)
	{
		int numFileBands, numFileSamples;

		inFile.read((char *)&numFileBands, sizeof(int));
		inFile.read((char *)&numFileSamples, sizeof(int));

		if(numFileBands!=numBands || numFileSamples!=numSamples)
		{
			LOG::Instance()->OutputMisc("directcoeffs.dat has different number of bands/samples, regenerating coefficients...");
			regenerateCoeffs=true;
			inFile.close();
		}
	}

	//If the file is good, read in the coefficients
	if(!regenerateCoeffs)
	{
		for(int i=0; i<numObjects; ++i)
		{
			SH_OBJECT * currentObject=objects[i];

			const int numVertices=currentObject->vertices.size();
			for(int j=0; j<numVertices; ++j)
			{
				SH_VERTEX & currentVertex=currentObject->vertices[j];

				inFile.read((char *)currentVertex.unshadowedCoeffs,
							numFunctions*sizeof(double));
				inFile.read((char *)currentVertex.shadowedCoeffs,
							numFunctions*sizeof(double));
			}
		}

		inFile.close();
		return true;
	}

	//Otherwise, regenerate the coefficients
	//Loop through the vertices and project the transfer function into SH space
	for(int i=0; i<numObjects; ++i)
	{
		SH_OBJECT * currentObject=objects[i];
		const int numVertices=currentObject->vertices.size();

		for(int j=0; j<numVertices; ++j)
		{
			SH_VERTEX & currentVertex=currentObject->vertices[j];

			//Clear the coefficients for this vertex
			for(int k=0; k<numFunctions; ++k)
			{
				currentVertex.unshadowedCoeffs[k]=0.0;
				currentVertex.shadowedCoeffs[k]=0.0;
			}

			//Loop through samples
			for(int k=0; k<numSamples; ++k)
			{
				//Calculate cosine term for this sample
				double dot=(double)samples[k].direction.DotProduct(currentVertex.normal);

				//Clamp to [0, 1]
				if(dot>0.0)
				{
					//Fill in a RAY structure for this sample
					RAY ray(currentVertex.position+2*EPSILON*currentVertex.normal,
							samples[k].direction);

					//See if the ray is blocked by any object
					bool rayBlocked=IsRayBlocked(ray);

					//Add the contribution of this sample to the coefficients
					for(int l=0; l<numFunctions; ++l)
					{
						double contribution=dot*samples[k].shValues[l];

						currentVertex.unshadowedCoeffs[l]+=contribution;

						if(!rayBlocked)
							currentVertex.shadowedCoeffs[l]+=contribution;
					}
				}
			}

			//Rescale the coefficients
			for(int k=0; k<numFunctions; ++k)
			{
				currentVertex.unshadowedCoeffs[k]*=4*M_PI/numSamples;
				currentVertex.shadowedCoeffs[k]*=4*M_PI/numSamples;
			}

			glClear(GL_COLOR_BUFFER_BIT);

			font.StartTextMode();

			font.Print(	WINDOW::Instance()->width/2-225, WINDOW::Instance()->height/2-40,
						"Calculating Transfer Function SH Coefficients");

			font.Print(	WINDOW::Instance()->width/2-100, WINDOW::Instance()->height/2-20,
						"Direct Illumination");

			font.Print(	WINDOW::Instance()->width/2-65, WINDOW::Instance()->height/2,
						"Object %d of %d", i+1, numObjects);

			font.Print(	WINDOW::Instance()->width/2-30, WINDOW::Instance()->height/2+20,
						"%d/100", int(j*100.0f/numVertices));
			
			font.EndTextMode();
			
			WINDOW::Instance()->SwapBuffers();
		}
	}
	
	//Save the coefficients to a file
	std::ofstream outFile("directcoeffs.dat", std::ios::out | std::ios::binary | std::ios::trunc);

	//First save the number of bands and samples
	outFile.write((const char *)&numBands, sizeof(int));
	outFile.write((const char *)&numSamples, sizeof(int));

	//Loop through the vertices and save the coefficients for each
	for(int i=0; i<numObjects; ++i)
	{
		SH_OBJECT * currentObject=objects[i];

		const int numVertices=currentObject->vertices.size();
		for(int j=0; j<numVertices; ++j)
		{
			SH_VERTEX & currentVertex=currentObject->vertices[j];

			outFile.write(	(const char *)currentVertex.unshadowedCoeffs,
							numFunctions*sizeof(double));
			outFile.write(	(const char *)currentVertex.shadowedCoeffs,
							numFunctions*sizeof(double));
		}
	}

	outFile.close();

	return true;
}
