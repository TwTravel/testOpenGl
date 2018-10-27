//////////////////////////////////////////////////////////////////////////////////////////
//	SH_SCENE_IsRayBlocked.cpp
//	Is a ray within the scene blocked by an object?
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

bool SH_SCENE::IsRayBlocked(const RAY & ray)
{
	//Test against each object
	int numObjects=objects.size();

	for(int i=0; i<numObjects; ++i)
	{
		if(objects[i]->DoesRayHitObject(ray))
			return true;
	}

	return false;
}