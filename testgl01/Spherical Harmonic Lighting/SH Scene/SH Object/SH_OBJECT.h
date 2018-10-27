//////////////////////////////////////////////////////////////////////////////////////////
//	SH_OBJECT.h
//	Abstract base class for an object to be lit by SH lighting
//	Downloaded from: www.paulsprojects.net
//	Created:	2nd October 2003
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef SH_OBJECT_H
#define SH_OBJECT_H

class SH_OBJECT
{
public:
	//Does a given ray intersect this object?
	virtual bool DoesRayHitObject(const RAY & ray)=0;

	//Geometry
	std::vector <SH_VERTEX> vertices;
	std::vector <GLuint> indices;
};

#endif