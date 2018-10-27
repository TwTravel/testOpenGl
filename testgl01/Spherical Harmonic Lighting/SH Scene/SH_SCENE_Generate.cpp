//////////////////////////////////////////////////////////////////////////////////////////
//	SH_SCENE_Generate.cpp
//	Generate the scene to be lit by SH lighting
//	Downloaded from: www.paulsprojects.net
//	Created:	2nd October 2003
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#include <vector>
#include "../GL files/glee.h"
#include "../Maths/Maths.h"
#include "../Log/LOG.h"
#include "../Bounding Volumes/Bounding Volumes.h"
#include "../Ray/RAY.h"
#include "SH_SCENE.h"

bool SH_SCENE::Generate(void)
{
	//Generate the individual objects within the scene, and add pointers to them to the vector
	


	//White sphere
	SH_SPHERE * sphere=new SH_SPHERE;
	if(!sphere)
	{
		LOG::Instance()->OutputError("Unable to create space for sphere");
		return false;
	}

	if(!sphere->Init(VECTOR3D(0.0f, 0.0f, 1.0f), 1.0f, 32, 16, white))
		return false;

	objects.push_back(sphere);



	//Red torus
	SH_TORUS * redTorus=new SH_TORUS;
	if(!redTorus)
	{
		LOG::Instance()->OutputError("Unable to create space for red torus");
		return false;
	}

	if(!redTorus->Init(VECTOR3D(0.0f,-2.0f, 0.5f), 0.5f, 1.0f, 16, 32, red))
		return false;

	objects.push_back(redTorus);



	//Green torus
	SH_TORUS * greenTorus=new SH_TORUS;
	if(!greenTorus)
	{
		LOG::Instance()->OutputError("Unable to create space for green torus");
		return false;
	}

	if(!greenTorus->Init(VECTOR3D(-2.0f, 0.0f, 0.5f), 0.5f, 1.0f, 16, 32, green))
		return false;

	objects.push_back(greenTorus);



	//Base
	SH_AA_BOX * base=new SH_AA_BOX;
	if(!base)
	{
		LOG::Instance()->OutputError("Unable to create space for base");
		return false;
	}

	if(!base->Init(	VECTOR3D(-3.0f,-3.0f,-0.1f), VECTOR3D(2.1f, 2.1f, 0.0f), 80, 80, 2, cyan))
		return false;

	objects.push_back(base);


	//Walls
	SH_AA_BOX * pxWall=new SH_AA_BOX;
	SH_AA_BOX * pyWall=new SH_AA_BOX;
	
	if(!pxWall || !pyWall)
	{
		LOG::Instance()->OutputError("Unable to create space for walls");
		return false;
	}

	if(!pxWall->Init(VECTOR3D(2.0f, -3.0f,-0.1f), VECTOR3D(2.1f, 2.1f, 2.0f), 2, 80, 32, cyan))
		return false;

	if(!pyWall->Init(VECTOR3D(-3.0f, 2.0f,-0.1f), VECTOR3D(2.1f, 2.1f, 2.0f), 80, 2, 32, cyan))
		return false;

	objects.push_back(pxWall);
	objects.push_back(pyWall);

	return true;
}
