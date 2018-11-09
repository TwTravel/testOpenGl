// GamaControl.h: interface for the CGamaControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMACONTROL_H__D6B2F027_6D7A_47F1_BE1E_1D264F0A1B27__INCLUDED_)
#define AFX_GAMACONTROL_H__D6B2F027_6D7A_47F1_BE1E_1D264F0A1B27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <windows.h>
#include <gl\gl.h>
class CGamaControl  
{
public:
	float GetGama();
	void Apply();
	void SetGama(float f);
	CGamaControl();
	virtual ~CGamaControl();

private:
	float m_gama;

};

#endif // !defined(AFX_GAMACONTROL_H__D6B2F027_6D7A_47F1_BE1E_1D264F0A1B27__INCLUDED_)
