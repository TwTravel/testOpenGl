//////////////////////////////////////////////////////////////////////////////////////////
//	GenerateSamples.cpp
//	Fill the array of samples using jittered stratified sampling
//	Downloaded from: www.paulsprojects.net
//	Created:	21st September 2003
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#include <stdlib.h>
#include "../Maths/Maths.h"
#include "../Log/LOG.h"
#include "SAMPLE.h"
#include "GenerateSamples.h"
#include "SH.h"

bool GenerateSamples(int sqrtNumSamples, int numBands, SAMPLE * samples)
{
	int numSamples=sqrtNumSamples*sqrtNumSamples;
	int numFunctions=numBands*numBands;

	//Create space for the SH values in each sample
	for(int i=0; i<numSamples; ++i)
	{
		samples[i].shValues=new double[numFunctions];
		if(!samples[i].shValues)
		{
			LOG::Instance()->OutputError("Unable to allocate space for SH values in samples");
			return false;
		}
	}

	int index=0;

	for(int i=0; i<sqrtNumSamples; ++i)
	{
		for(int j=0; j<sqrtNumSamples; ++j)
		{
			//Generate the position of this sample in [0, 1)x[0, 1)
			double x=(i+((double)rand()/RAND_MAX))/sqrtNumSamples;
			double y=(j+((double)rand()/RAND_MAX))/sqrtNumSamples;

			//Convert to spherical polars
			double theta=2.0*acos(sqrt(1.0-x));
			double phi=2.0*M_PI*y;
			
			samples[index].theta=theta;
			samples[index].phi=phi;

			//Convert to cartesians
			samples[index].direction.Set(	float(sin(theta)*cos(phi)),
											float(sin(theta)*sin(phi)),
											float(cos(theta)));

			//Compute SH coefficients for this sample
			for(int l=0; l<numBands; ++l)
			{
				for(int m=-l; m<=l; ++m)
				{
					int index2=l*(l+1)+m;

					samples[index].shValues[index2]=SH(l, m, theta, phi);
				}
			}
			
			++index;
		}
	}

	return true;
}
