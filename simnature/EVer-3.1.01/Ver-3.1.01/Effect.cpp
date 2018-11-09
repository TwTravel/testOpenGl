/******************************************************************   
  模块名称:
       Effect.cpp
   目的: 
      效果器的实现文件。

   作者:
        潘李亮 2003 - 5

  所有的后期特殊效果处理代码，
  在本程序里。所有的效果以MotionBlur开始，形成一条效果链。
  所有的效果处理器都是一个CEffect的子类
******************************************************************/


#include <math.h>
#include "Effect.h"


CEffect::CEffect()
{
	m_isEffected = FALSE; 
}

CEffect::~CEffect()
{

}

COverBurtEff::COverBurtEff()
{
	m_val = 0.1;
}

BOOL COverBurtEff::Render(GLuint texID, float tx, float ty,int view_x,int view_y)
{
	if( ! m_isEffected )
		return FALSE;
	glEnable(GL_BLEND);
	glColor4f(0,0,0,m_val);
	glBlendFunc(GL_ONE,GL_SRC_ALPHA);
	for(int i=0;i<3;i++)
	{
		glScalef(0.999,0.999,1);
		glBegin(GL_QUADS);
			glTexCoord2f(0 ,0);	    glVertex3f(-1,-1,-5);
			glTexCoord2f(tx,0);	    glVertex3f(1,-1,-5);
			glTexCoord2f(tx,ty);	glVertex3f(1,1,-5);
			glTexCoord2f(0 ,ty);	glVertex3f(-1,1,-5);
		glEnd();
	}
	return TRUE;
}



float COverBurtEff::GetValue()
{
	return m_val;
}

void COverBurtEff::SetValue(float v)
{
	m_val = v;
	if(m_val > 1.0)
		m_val  = 1.0;
	if(m_val < 0.0)
		m_val = 0.0;
}

BOOL  CMapPilotEff::Render(GLuint texID, float tx, float ty,int view_x,int view_y)
{
	if( ! m_isEffected )
		return FALSE;

	glBindTexture(GL_TEXTURE_2D,m_TerrTex);
	glEnable(GL_BLEND);
	glColor4f(0,0,0,0.6);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

		glBegin(GL_QUADS);
			glTexCoord2f(0 ,0);	    glVertex3f(0.5,0.5,-5);
			glTexCoord2f(1,0);	    glVertex3f(1  ,0.5,-5);
			glTexCoord2f(1,1);	    glVertex3f(1  ,1  ,-5);
			glTexCoord2f(0 ,1);	    glVertex3f(0.5,1  ,-5);
		glEnd();

	glDisable(GL_TEXTURE_2D);

	float x = 0.5* m_pCmInfo->pos.x/m_w + 0.5;
    float y = 0.5* m_pCmInfo->pos.y/m_h + 0.5;
	glPointSize(2);
	glDisable(GL_BLEND);
	glColor3f(1.0,0,0);
	glBegin(GL_POINTS);
		glVertex3f(x,y,-5);
	glEnd();
	glBindTexture(GL_TEXTURE_2D,texID);
	return TRUE;
}


void CGamaEff::SetValue(float f)
{
	m_gama = f;
	if(f>2.0)
		m_gama=2.0;
	if(f<0)
		m_gama=0;
}

BOOL  CGamaEff::Render(GLuint texID, float tx, float ty,int view_x,int view_y)
{
	if( ! m_isEffected )
		return FALSE;

	if(m_gama == 1.0)
		return TRUE;

	//现在我们使用混合功能，把先前保留下来的图象和当前的渲染结果混合起来。
	if(m_gama<=1.0)
		glColor4f(0,0,0,m_gama);
	else
		glColor4f(m_gama-1.0,m_gama-1.0,m_gama-1.0,1.);
	
	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_ONE,GL_SRC_ALPHA);

	glBegin(GL_QUADS);
		glVertex3f(-1,-1,5);
		glVertex3f(1 ,-1,5);
		glVertex3f(1 ,1 ,5);
		glVertex3f(-1,1 ,5);
	glEnd();

	return TRUE;
}
BOOL  CSnipEff::Render(GLuint texID, float tx, float ty,int view_x,int view_y)
{
	if( ! m_isEffected )
		return FALSE;


	//现在我们使用混合功能，把先前保留下来的图象和当前的渲染结果混合起来。

	glBindTexture(GL_TEXTURE_2D,m_texID);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_ZERO,GL_SRC_COLOR);

	glBegin(GL_QUADS);
			glTexCoord2f(0 , 0);	    glVertex3f(-1 , -1, -5);
			glTexCoord2f(1 , 0);	    glVertex3f(1  , -1, -5);
			glTexCoord2f(1 , 1);		glVertex3f(1  , 1 , -5);
			glTexCoord2f(0 , 1);		glVertex3f(-1 , 1 , -5);
	glEnd();
	glDisable(GL_BLEND);
	return TRUE;
}
BOOL  CCrossEff::Render(GLuint texID, float tx, float ty,int view_x,int view_y)
{
	if( ! m_isEffected )
		return FALSE;

//	glEnable(GL_BLEND);
//	glBlendFunc(GL_ONE_MINUS_DST_COLOR,GL_ZERO);

	glColor3f(0,0,0);
	//现在我们使用混合功能，把先前保留下来的图象和当前的渲染结果混合起来。
	glBegin(GL_LINES);
			glVertex3f(-0.7 , 0, -5);
			glVertex3f( 0.7 , 0, -5);
			glVertex3f(0 , -0.7 , -5);
			glVertex3f(0 , 0.7 , -5);
	glEnd();
	return TRUE;
}
CScrapEff::CScrapEff()
{
	for(int i=0;i<20;i++)
		for(int j=0;j<20;j++)
		{
			_patch[i][j].x = (i-10)/10.0;
			_patch[i][j].y = (j-10)/10.0;
			_patch[i][j].angle_x = rand()%360;
			_patch[i][j].angle_y = rand()%360;
			_patch[i][j].angle_y = rand()%360;
		}
}
BOOL CScrapEff::Render(GLuint texID, float tcx, float tcy,int view_x,int view_y)
{
	if( ! m_isEffected )
		return FALSE;

	if(m_life == 0)
		return FALSE;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texID);
	//刚开始一个效果
	if(m_life == 400)
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0,0,0,0,0,view_x,view_y);

	m_life --;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glDisable(GL_BLEND);

	float tx,ty;
	for(int i =0;i<20;i++)
		for(int j =0;j<20;j++)
		{
			float s = -(9.0/400)*m_life + 10;

			glLoadIdentity();
			glScalef(s,s,1);
			glTranslatef(_patch[i][j].x,_patch[i][j].y,0);
			glRotatef(_patch[i][j].angle_z,0,0,1);
			glRotatef(_patch[i][j].angle_x,1,0,0);
			glRotatef(_patch[i][j].angle_y,0,1,0);
			_patch[i][j].angle_z += (rand()%45) /5;
			_patch[i][j].angle_x += (rand()%45) /5;
			_patch[i][j].angle_y += (rand()%45) /5;
		
			if(i>=10)
				_patch[i][j].x += (sqrt(i - 9.5)* (rand()%15) /1000.0 +0.0004);
			else
				_patch[i][j].x -= (sqrt(9.5 - i)* (rand()%15) /1000.0 +0.0004);

			if(j>=10)
				_patch[i][j].y += (sqrt(j - 9.5)* (rand()%15) /1000.0 +0.0004);
			else
				_patch[i][j].y -= (sqrt(9.5 - j)* (rand()%15) /1000.0 +0.0004);

			tx = (i)/20.0 * tcx;
			ty = (j)/20.0 * tcy;
			glBegin(GL_QUADS);
				glTexCoord2f(tx     ,ty)      ; glVertex3f(0  ,0  ,0);
				glTexCoord2f(tx+0.05*tcx,ty)      ; glVertex3f(0.1,0  ,0);
				glTexCoord2f(tx+0.05*tcx,ty+0.05*tcy) ; glVertex3f(0.1,0.1,0);
				glTexCoord2f(tx     ,ty+0.05*tcy) ; glVertex3f(0  ,0.1,0);
			glEnd();
		}

	glPopMatrix();
	return TRUE;

}

void CScrapEff::Scrap()
{
	for(int i=0;i<20;i++)
		for(int j=0;j<20;j++)
		{
			_patch[i][j].x = (i-10)/10.0;
			_patch[i][j].y = (j-10)/10.0;
			_patch[i][j].angle_x = 0;//rand()%360;
			_patch[i][j].angle_y = 0;//rand()%360;
			_patch[i][j].angle_z = 0;//rand()%360;
		}
    
	m_life = 400;
}
