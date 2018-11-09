
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "OpenGLCom.h"
#include "SkyBox.h"

void CSkyBox::InitSkyBox(float wx,float wy,float h,int front,int back,int left,int right,int top,int bottom)
{
	m_back  = back;
	m_front = front;
	m_left  = left;
	m_right = right;
	m_top   = top;
	m_bottom= bottom;
	m_wx = wx;
    m_wy = wy;
	m_h  = h;

}
void CSkyBox::Draw(int start_x,int start_y,int start_h)
{

  glDepthMask(0);
    const float nh = start_h;
	const float h = m_h+start_h;
	float wx = m_wx;
	float wy = m_wy;
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);


	glMatrixMode(GL_PROJECTION);
	glPushMatrix();	
	glLoadIdentity();

	float f = m_pcm->GetCameraInfo()->far_dist;
	
	m_pcm->GetCameraInfo()->far_dist = 102499;
	m_pcm->Project(g_GL.m_view_factor);
    //gluPerspective(g_GL.m_eye_angle,float(g_GL.m_view_width)/g_GL.m_view_height,0.1,102499);
  
	m_pcm->GetCameraInfo()->far_dist = f;

	glMatrixMode(GL_PROJECTION);

	glBindTexture(GL_TEXTURE_2D,m_top);
	glBegin(GL_QUADS);
	   glTexCoord2f(1,0);
		glVertex3f(0,0,h);
	   glTexCoord2f(1,1);
		glVertex3f(0,wy,h);
       glTexCoord2f(0,1);
		glVertex3f(wx,wy,h);
	   glTexCoord2f(0,0);
		glVertex3f(wx,0,h);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,m_bottom);
	glBegin(GL_QUADS);
	   glTexCoord2f(1,0);
		glVertex3f(0,0,nh);

	   glTexCoord2f(0,0);
		glVertex3f(wx,0,nh);

       glTexCoord2f(0,1);
		glVertex3f(wx,wy,nh);

	   glTexCoord2f(1,1);
		glVertex3f(0,wy,nh);


	glEnd();

    glBindTexture(GL_TEXTURE_2D,m_left);
	glBegin(GL_QUADS);
	   glTexCoord2f(0,0);
		glVertex3f(0,0,nh);
	   glTexCoord2f(1,0);
		glVertex3f(0,wy,nh);
       glTexCoord2f(1,1);
		glVertex3f(0,wy,h);
	   glTexCoord2f(0,1);
		glVertex3f(0,0,h);
	glEnd();

    glBindTexture(GL_TEXTURE_2D,m_back);
	glBegin(GL_QUADS);
	   glTexCoord2f(0,0);
		glVertex3f(0,wy,nh);
	   glTexCoord2f(1,0);
		glVertex3f(wx,wy,nh);
       glTexCoord2f(1,1);
		glVertex3f(wx,wy,h);
	   glTexCoord2f(0,1);
		glVertex3f(0,wy,h);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,m_right);
	glBegin(GL_QUADS);
	   glTexCoord2f(1,0);
		glVertex3f(wx,0,nh);
	   glTexCoord2f(1,1);
		glVertex3f(wx,0,h);
       glTexCoord2f(0,1);
		glVertex3f(wx,wy,h);
	   glTexCoord2f(0,0);
		glVertex3f(wx,wy,nh);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,m_front);
	glBegin(GL_QUADS);
	   glTexCoord2f(1,0);
		glVertex3f(0,0,nh);
	   glTexCoord2f(1,1);
		glVertex3f(0,0,h);
       glTexCoord2f(0,1);
		glVertex3f(wx,0,h);
	   glTexCoord2f(0,0);
		glVertex3f(wx,0,nh);
	glEnd();


	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
  glDepthMask(1);
}