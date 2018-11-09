/**********************************************************
   模块名称:
       GamaControl.cpp
   目的: 
       提供一个伪Gama控制器。

   作者:
        潘李亮2003 - 5

**********************************************************/
#include "GamaControl.h"
#include "OpenGLCom.h"

CGamaControl::CGamaControl()
{
	m_gama = 1.0;
}

CGamaControl::~CGamaControl()
{

}

void CGamaControl::SetGama(float f)
{
	m_gama = f;
	if(f>2.0)
		m_gama=2.0;
	if(f<0)
		m_gama=0;
}

void CGamaControl::Apply()
{

	if(m_gama == 1.0)
		return ;
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0,1.0,0,1.0,-100,100);
    
	//单位的视图矩阵。我们不需要任何的变换，因为是平面的图象操作。
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();


    glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
    glDisable(GL_ALPHA_TEST);
	glDisable(GL_LIGHTING);

	//现在我们使用混合功能，把先前保留下来的图象和当前的渲染结果混合起来。
	if(m_gama<=1.0)
		glColor4f(0,0,0,m_gama);
	else
		glColor4f(m_gama-1.0,m_gama-1.0,m_gama-1.0,1.);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE,GL_SRC_ALPHA);

	glPushAttrib(GL_POLYGON_BIT); 
	glPolygonMode(GL_FRONT,GL_FILL);
	glPolygonMode(GL_BACK,GL_FILL);
	glBegin(GL_QUADS);
		glVertex3f(0,0,5);
		glVertex3f(1,0,5);
		glVertex3f(1,1,5);
		glVertex3f(0,1,5);
	glEnd();

    glPopAttrib();
   
	//恢复投影矩阵和视图矩阵。
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

float CGamaControl::GetGama()
{
	return m_gama;
}
