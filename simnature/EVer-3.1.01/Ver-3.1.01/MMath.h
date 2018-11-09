/**********************************************************
  SimNature Ver1.0.0
  Copyright : Panliliang (Stanly Lee)
  2002 - 2003
  Alright reserved!
  All Noncommeric use is Free!

  ��������Ȩ���� 
             2003-2003
  ����������з���ҵ��ʹ��Ϊ��Ѵ��롣

  E- Mial: Panliliang@etang.com
		   Xheartblue@etang.com 




  ģ������:
  ����������

  �汾��
  ���ߣ�

**********************************************************/
#ifndef _GL_MATH_
#define _GL_MATH_


#include "OpenGLCom.h"

typedef struct tag_Vector
{
	float x,y,z,w;
}MVECTOR,MPOINT;

typedef float (MMATRIX)[4][4];
MVECTOR operator -(MVECTOR& v1,MVECTOR& v2);
//MVECTOR operator +(MVECTOR& v1,MVECTOR& v2);
class CMath
{
public:
	float GetCosFromSin(float s);
	float GetSinFromCos(float);
	float Sin(int angle);
	float Cos(int angle);
   ~CMath();
    CMath();
	void    CrossNormal(MVECTOR& vec1,MVECTOR& vec2, MVECTOR& normal);
	void    Cross(MVECTOR& vec1,MVECTOR & vec2,MVECTOR& out);

	void    Normalize(MVECTOR& normal);
	void    MultVector(MVECTOR& vec1 ,   MVECTOR&  vec2, MVECTOR& vout);
	void    VectorAdd( MVECTOR& vec1 ,   MVECTOR&  vec2, MVECTOR& vout);
	float   LenVector( MVECTOR& vec);
	float   GetVectorClamp(MVECTOR& vec1,MVECTOR& vec2);

//��������������֮�����ת
	void    Rotate(MVECTOR& vn, MVECTOR& vout,float theta);
//һ������һ���ߵ���ת
	void    RotatePoint(MVECTOR& vn,MPOINT& v,float theta);

//4���ֽڶ���
	int     Round4Byte(int n);
	BOOL   IsMagicNumber(int n)
	{
		return !(  (( n - 1)& n )  != 0);
	}

private:
	float*  m_cos;
    float*  m_sin;
};

extern CMath Math;
#endif


