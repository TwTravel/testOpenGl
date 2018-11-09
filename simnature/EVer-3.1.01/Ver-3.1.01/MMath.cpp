/*
   ģ������:
       GLmath.cpp
   Ŀ��: 
       �ṩͨ�õ�ͼ��ѧ��ѧ���㹦�ܣ��������ĳ˷�����;

   ����:
        ������ HeartBlue
		       XpertSoft HeartBlue 2002/9/12
			   Allright reserved
			   ��Ȩ���� 
*/

#include "MMath.h"
#include <math.h>

CMath Math;
CMath::CMath()
{
 m_cos = new float[361];
 m_sin = new float[361];
 for(int i=0;i<=360;i++)
 {
	 m_cos[i]=cos( i*3.14159/180);
	 m_sin[i]=sin( i*3.14159/180);
 }
}

CMath::~CMath()
{
	delete[] m_cos;
	delete[] m_sin;
}

MVECTOR operator -(MVECTOR& v1,MVECTOR& v2)
{
	MVECTOR temp;
	temp.x= v1.x - v2.x;
	temp.y= v1.y - v2.y;
	temp.z= v1.z - v2.z;
	return temp;
}
//����󣬵�λ��
void  CMath::CrossNormal(MVECTOR & vec1,MVECTOR & vec2,MVECTOR& normal)
{
	normal.x=vec1.y*vec2.z-vec1.z*vec2.y;
	normal.y=vec1.z*vec2.x-vec1.x*vec2.z;
	normal.z=vec1.x*vec2.y-vec1.y*vec2.x;
	float m= sqrt(normal.x*normal.x+normal.y*normal.y+normal.z*normal.z);
	if(m==0)
		return;
	normal.x/=m;
	normal.y/=m;
	normal.z/=m;
}

//����
void  CMath::Cross(MVECTOR& vec1,MVECTOR& vec2,MVECTOR& vout)
{
	vout.x = vec1.y*vec2.z - vec1.z*vec2.y;
	vout.y = vec1.z*vec2.x - vec1.x*vec2.z;
	vout.z = vec1.x*vec2.y - vec1.y*vec2.x;
}

//��λ��
void CMath::Normalize(MVECTOR& normal)
{
	float m= sqrt(normal.x*normal.x+normal.y*normal.y+normal.z*normal.z);
	if(m==0)
		return;
	normal.x/=m;
	normal.y/=m;
	normal.z/=m;
}

//����
float CMath::LenVector(MVECTOR& vec)
{
		return sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}

//����ʸ�����
void CMath::MultVector(MVECTOR& vec1,MVECTOR& vec2,MVECTOR& vout)
{
	vout.x  = vec1.y*vec2.z-vec1.z*vec2.y;
	vout.y  = vec1.z*vec2.x-vec1.x*vec2.z;
	vout.z  = vec1.x*vec2.y-vec1.y*vec2.x;
}
//����ʸ�����
void CMath::VectorAdd(MVECTOR& vec1,MVECTOR & vec2,MVECTOR & vout)
{
	vout.x = vec1.x + vec2.x;
	vout.y = vec1.y + vec2.y;
	vout.z = vec1.z + vec2.z;
}

//������ʸ���ļн�
float   CMath::GetVectorClamp(MVECTOR & vec1,MVECTOR & vec2)
{
	float c = vec1.x*vec2.x + vec1.y*vec2.y + vec1.z*vec2.z;

	if(LenVector(vec1)==0)
		return 0;
	if(LenVector(vec2)==0)
		return 0;
	c = c/ LenVector(vec1)*LenVector(vec2);
	float a = acos(c);
	return a*57.2958;//return a*180/3.14159;
}

//һ������������һ��������ת theta�Ƕȵļ���
void  CMath::Rotate(MVECTOR& vecn,MVECTOR& vec,float theta)
{
  float s = sin(theta*3.14159/180);
  float cosine = cos(theta*3.14159/180);
  float a= vecn.x * s;
  float b= vecn.y * s;
  float c= vecn.z * s;
  float x,y,z;
  x = vec.z*b + vec.x*cosine-vec.y*c;
  y = vec.x*c + vec.y*cosine-vec.z*a;
  z = vec.y*a + vec.z*cosine-vec.x*b;
  vec.x=x;
  vec.y=y;
  vec.z=z;
}

void  CMath::RotatePoint(MVECTOR& vecn, MVECTOR& vec,float theta)
{
//���������ĵ��
   float k=vecn.x*vec.x + vecn.y*vec.y + vecn.z*vec.z;
//ͶӰ
   MVECTOR  n2 = { k*vecn.x , k*vecn.y, k*vecn.z};

   MVECTOR  r  = { vec.x-n2.x , vec.y-n2.y , vec.z-n2.z };
   float lr=sqrt( r.x*r.x + r.y*r.y + r.z*r.z);
   //��λ��r����
   Normalize(r);
   //��תr.
   Rotate(vecn,r,theta);
   //�ָ�r�ĳ���
   r.x *= lr;
   r.y *= lr;
   r.z *= lr;
   vec.x= r.x + n2.x;
   vec.y= r.y + n2.y;
   vec.z= r.z + n2.z;
   
}


int   CMath::Round4Byte(int n)
{
	if ( ( n%4)==0 )
		return n;
    else
	{
		return (n+ 4-(n%4));
	}
}

float CMath::Cos(int angle)
{
	return m_cos[angle];
}

float CMath::Sin(int angle)
{
	return m_sin[angle];
}

float CMath::GetSinFromCos(float c)
{
	if(c<0) c= -c;
	for(int i=0;i<90;i++)
	{
		if(c<=m_cos[i] && c>m_cos[i+1])
			return m_sin[i];
	}
	return 0;
}

float CMath::GetCosFromSin(float c)
{
	if(c<0) c= -c;
	for(int i=0;i<90;i++)
	{
		if(c>=m_sin[i] && c<m_sin[i+1])
			return m_cos[i];
	}
	return 0;
}



