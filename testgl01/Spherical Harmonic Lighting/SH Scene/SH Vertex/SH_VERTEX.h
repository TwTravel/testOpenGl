//////////////////////////////////////////////////////////////////////////////////////////
//	SH_VERTEX.h
//	Class for a vertex to be lit by SH lighting
//	Downloaded from: www.paulsprojects.net
//	Created:	2nd October 2003
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef SH_VERTEX_H
#define SH_VERTEX_H

class SH_VERTEX
{
public:
	VECTOR3D position;
	VECTOR3D normal;

	//Diffuse material color
	COLOR diffuseMaterial;

	//SH coefficients for transfer function
	double * unshadowedCoeffs;
	double * shadowedCoeffs;

	//Color of vertex including lighting
	COLOR litColor;

	SH_VERTEX()	: unshadowedCoeffs(NULL), shadowedCoeffs(NULL)
	{}
	~SH_VERTEX()
	{
		if(unshadowedCoeffs)
			delete [] unshadowedCoeffs;
		unshadowedCoeffs=NULL;

		if(shadowedCoeffs)
			delete [] shadowedCoeffs;
		shadowedCoeffs=NULL;
	}
};

#endif