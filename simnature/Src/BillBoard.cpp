/*
   ģ������:
       BillBoard.cpp
   Ŀ��: 
      �ṩ��BillBoard��֧��;

   ����:
        ������ HeartBlue
		       XpertSoft HeartBlue 2002/9/29
			   Allright reserved

 How to use this Moduale?

 1)ΪBillBoardָ��һ����Ӱ������BillBoard�Ļ��ƻص�����
 2)ΪBillBoard�Ļ��ƻص�����
 3)��bbCreate��������һ��BillBoard���ú��������Զ�ָ��BillBoard�ķ���
 4)ָ����BillBoard�ڳ����е�λ��
 5)�ڻ�BillBoardǰ����bbBeginDraw����
 6)����BillBoard

         ������ 2002/9/29

 �汾 2
   �����Ѿ���ת���� C++�ĸ�ʽ
   ���е�BillBoard��BillBoard Manager����
        ������ 2003/4/21
*/


#include "CommGLHeader.h"

void C2DBillBoard::DefDraw()
{
	glBegin(GL_QUADS);
    glTexCoord2f(0,0);	glVertex3f(-m_width/2,0,0);
	glTexCoord2f(1,0);  glVertex3f(m_width/2,0,0);
	glTexCoord2f(1,1);  glVertex3f(m_width/2,0,m_height);
	glTexCoord2f(0,1);  glVertex3f(-m_width/2,0,m_height);
	glEnd();
}

C2DBillBoard::C2DBillBoard(BILLBOARDDRAW pfun)
{
	//����BillBord�Ļ��ƺ���
	m_pfnDraw =pfun;
}



void C2DBillBoard::Draw(float theta,MVECTOR vUp)
{
	//����һ���� ��Ӱ�������Billboard����Ĳ��
    glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	//��ת��Ӱ����ʹ����׼ �����ķ���
	glTranslatef(m_x,m_y,m_z);
	glRotated(theta,vUp.x,vUp.y,vUp.z);
    if(m_pfnDraw) 
	   m_pfnDraw(1);
	else
	   DefDraw();

    glPopMatrix();
}

void C2DBillBoard::SetDraw(BILLBOARDDRAW pfun)
{
	m_pfnDraw = pfun; 
}