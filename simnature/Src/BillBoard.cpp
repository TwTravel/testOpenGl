/*
   模块名称:
       BillBoard.cpp
   目的: 
      提供对BillBoard的支持;

   作者:
        潘李亮 HeartBlue
		       XpertSoft HeartBlue 2002/9/29
			   Allright reserved

 How to use this Moduale?

 1)为BillBoard指定一个摄影机，和BillBoard的绘制回调函数
 2)为BillBoard的绘制回调函数
 3)用bbCreate函数创建一个BillBoard，该函数可以自动指定BillBoard的方向
 4)指定该BillBoard在场景中的位置
 5)在画BillBoard前调用bbBeginDraw函数
 6)绘制BillBoard

         潘李亮 2002/9/29

 版本 2
   代码已经被转化成 C++的格式
   所有的BillBoard由BillBoard Manager管理
        潘李亮 2003/4/21
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
	//设置BillBord的绘制函数
	m_pfnDraw =pfun;
}



void C2DBillBoard::Draw(float theta,MVECTOR vUp)
{
	//保存一个由 摄影机方向和Billboard方向的叉积
    glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	//旋转摄影机，使它对准 公告板的方向
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