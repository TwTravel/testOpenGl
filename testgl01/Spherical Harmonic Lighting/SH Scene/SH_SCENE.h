//////////////////////////////////////////////////////////////////////////////////////////
//	SH_SCENE.h
//	Class for the scene to be lit by SH lighting
//	Downloaded from: www.paulsprojects.net
//	Created:	2nd October 2003
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef SH_SCENE_H
#define SH_SCENE_H

#include "SH Vertex/SH_VERTEX.h"

#include "SH Object/SH_OBJECT.h"
#include "SH Object/SH_SPHERE.h"
#include "SH Object/SH_TORUS.h"
#include "SH Object/SH_AA_BOX.h"

#include "../Samples/SAMPLE.h"

class SH_SCENE
{
public:
	//Generate the scene
	bool Generate(void);

	//Generate the SH coefficients for the direct lighting transfer functions
	bool GenerateDirectCoeffs(int numBands, int numSamples, SAMPLE * samples);

	//Is a ray within the scene blocked by an object?
	bool IsRayBlocked(const RAY & ray);

	//Objects making up the scene
	std::vector <SH_OBJECT *> objects;
};

#endif