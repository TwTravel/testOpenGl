/**********************************************************
  SimNature Ver1.0.0
  Copyright : Panliliang (Stanly Lee)
  2002 - 2003
  Alright reserved!
  All Noncommeric use is Free!

  潘李亮版权所有 
             2003-2003
  本程序的所有非商业化使用为免费代码。

  E- Mial: Panliliang@etang.com
		   Xheartblue@etang.com 




  模块名称:
  功能描述：

  版本：
  作者：

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

//两个正交的向量之间的旋转
	void    Rotate(MVECTOR& vn, MVECTOR& vout,float theta);
//一个点绕一条线的旋转
	void    RotatePoint(MVECTOR& vn,MPOINT& v,float theta);

//4个字节对齐
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


