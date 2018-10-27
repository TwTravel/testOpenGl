//////////////////////////////////////////////////////////////////////////////////////////
//	Light.cpp
//	Return the brightness of the light source, given spherical polar direction
//	Downloaded from: www.paulsprojects.net
//	Created:	21st September 2003
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#include "../Maths/Maths.h"
#include "Light.h"

/*double Light(double theta, double phi)
{
	double temp1=sin(2*theta);

	if(temp1<0.0)
		temp1=0.0;

	double temp2=sin(phi/2-M_PI/8);

	if(temp2<0.0)
		temp2=0.0;

	double temp3=8.0*temp1*temp2-6.0;

	if(temp3<0.0)
		temp3=0.0;

	return temp3;
}*/


double Light(double theta, double phi)
{
	return (theta<M_PI/6) ? 1 : 0;
}