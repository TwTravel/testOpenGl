#include "CommGLHeader.h"
#include "Console.h"

void CLensFlar::Release()
{
   m_MainNodes.clear();
   m_SubNodes.clear();
}

void CLensFlar::SetLocation(float x,float y,float z)
{
    m_loca_x = x;
	m_loca_y = y;
	m_loca_z = z;
}
void CLensFlar::AddSubNode(float size,float pos,int texID)
{

	LFNODE node={pos,size,texID};
    m_SubNodes.push_back(node); 
}
void CLensFlar::AddMainNode(float size,int texID)
{
	LFNODE node={0,size,texID};
    m_MainNodes.push_back(node); 
}
void CLensFlar::Draw()
{

	if(IsBackOfViewer())
		return;

	float lx,ly;//中心眩光环到屏幕中心的距离
	int x,y;//一个眩光环的位置。
	int cx,cy;//屏幕的中心点
	int i;
	float z;
	GLdouble mProj[16];
	GLdouble mView[16];
	GLint mPort[4];
	GLdouble wx,wy,wz;

	glGetDoublev(GL_PROJECTION_MATRIX,mProj);
	glGetDoublev(GL_MODELVIEW_MATRIX,mView);
	glGetIntegerv(GL_VIEWPORT,mPort);

    gluProject(m_loca_x,m_loca_y,m_loca_z,
		       mView,mProj,mPort,
               &wx,&wy,&wz);
	if(wx<mPort[0] || wx>mPort[2])return;
	if(wy<mPort[1] || wy>mPort[3])return;

	//检测 眩光的中心点是不是被 东西档住了，加上这两行就可以防止太阳光穿过山和树^_^
	glReadPixels(wx,wy,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&z);

    if(z < wz && z !=1.0 )return;
	if(wz<0) return ;
//	Console.Log("lens z-bufer: %f, project z: %f",z,wz);
	//设置眩光体屏幕上的位置和 屏幕的中心位置
	cx = mPort[2]/2.;
	cy = mPort[3]/2.;
    lx = cx-wx ;
	ly = cx-wy ;
	//准备好新的2D平面投影矩阵
	glPushAttrib(GL_ENABLE_BIT);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	glLoadIdentity();
	glOrtho(0,mPort[2],0,mPort[3],-100,100);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
//关闭雾，和深度检测
    glDisable(GL_FOG);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_COLOR,GL_ONE);
 //画主眩光部分   
	float a =atan(lx/ly)*180/3.14;
	glColor3f(1.0,.6,0.);
 	for(i=0;i<m_MainNodes.size();i++)
	{
		glTranslated(wx,wy,0);
		glRotated((i+4)/4.*a,0,0,1);
		glTranslated(-wx,-wy,0);
		glBindTexture(GL_TEXTURE_2D,m_MainNodes[i].texID);  
        glBegin(GL_QUADS);
		   glTexCoord2f(0,0);
			glVertex3f(wx - m_MainNodes[i].size,wy - m_MainNodes[i].size,-10);
           glTexCoord2f(1,0);
			glVertex3f(wx + m_MainNodes[i].size,wy - m_MainNodes[i].size,-10); 
           glTexCoord2f(1,1);
			glVertex3f(wx + m_MainNodes[i].size,wy + m_MainNodes[i].size,-10);
		   glTexCoord2f(0,1);	
			glVertex3f(wx - m_MainNodes[i].size,wy + m_MainNodes[i].size,-10); 
		glEnd();
	}
	glLoadIdentity();

//画次级眩光部分
	for(i=0;i<m_SubNodes.size();i++)
	{
        x = wx + lx * m_SubNodes[i].pos; 
        y = wy + ly * m_SubNodes[i].pos;
		glBindTexture(GL_TEXTURE_2D,m_SubNodes[i].texID);  
        glBegin(GL_QUADS);
		   glTexCoord2f(0,0);
		   glVertex3f(x - m_SubNodes[i].size , y - m_SubNodes[i].size ,-10);
           glTexCoord2f(1,0);
		   glVertex3f(x + m_SubNodes[i].size , y - m_SubNodes[i].size ,-10);
           glTexCoord2f(1,1);
		   glVertex3f(x + m_SubNodes[i].size , y + m_SubNodes[i].size ,-10);
		   glTexCoord2f(0,1);	
		   glVertex3f(x - m_SubNodes[i].size , y + m_SubNodes[i].size ,-10);
		glEnd();
	}
//恢复原来的矩阵
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}


void CLensFlar::SetCamera(CCamera *pCamera)
{
    m_pCamera = pCamera->GetCameraInfo(); 
}

BOOL CLensFlar::IsBackOfViewer()
{
    float dx = m_loca_x - m_pCamera->pos.x;
	float dy = m_loca_y - m_pCamera->pos.y;
	float dz = m_loca_z - m_pCamera->pos.z;
	
	float dot_c = dx * (m_pCamera->target.x - m_pCamera->pos.x) ;
	dot_c += (m_pCamera->target.y - m_pCamera->pos.y) ;
	dot_c += (m_pCamera->target.z - m_pCamera->pos.z) ;

	if(dot_c<0)
		return true;
	else
		return false;
}
