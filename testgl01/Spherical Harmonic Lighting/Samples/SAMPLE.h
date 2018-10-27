//////////////////////////////////////////////////////////////////////////////////////////
//	SAMPLE.h
//	Class for a sample on the surface of the unit sphere
//	Downloaded from: www.paulsprojects.net
//	Created:	21st September 2003
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef SAMPLE_H
#define SAMPLE_H

class SAMPLE
{
public:
	//Spherical polar coords
	double theta;
	double phi;

	//Cartesian direction
	VECTOR3D direction;

	//Values of each SH function at this point
	double * shValues;

	SAMPLE()	:	shValues(NULL)
	{}
	~SAMPLE()
	{
		if(shValues)
			delete [] shValues;
		shValues=NULL;
	}
};

#endif


