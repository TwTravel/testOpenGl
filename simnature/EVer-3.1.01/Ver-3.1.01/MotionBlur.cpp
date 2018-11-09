#include "MotionBlur.h"
#include "OpenGLCom.h"
#include "Console.h"

CMotionBlur::CMotionBlur()
{
	m_bEffected = FALSE;
	m_blur_val  = 0.7;
	m_bIsCreated = FALSE;
}

CMotionBlur::~CMotionBlur()
{
	glDeleteTextures(1,&m_tex);
	m_Effects.clear();
}

void CMotionBlur::BeginRender()
{
	if(g_GL.m_view_height > m_view_port_y || g_GL.m_view_width > m_view_port_y)
	{
		if(m_bEffected == TRUE)
			glViewport(0,0,m_view_port_x,m_view_port_y);
		else
			glViewport(0,0,g_GL.m_view_width,g_GL.m_view_height);
	}
}

void CMotionBlur::Render()
{


	if(!m_bEffected)
	{
		OnlyRenderOtherEffect();
	    return ;
	}
  
	//Set the enviroment
	PreRender();
	//现在我们使用混合功能，把先前保留下来的图象和当前的渲染结果混合起来。
	glColor4f(1.0,1.0,1.0,m_blur_val);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//由于我们的纹理图象的大小是 1/1。但是窗口的比例却不是。
	//所有我们要切除没有渲染的部分（即黑的那部分）
	float tx = m_view_port_x/(float)m_tex_size_x;
	float ty = m_view_port_y/(float)m_tex_size_y;

	//把先前图象作为一个纹理画在一个矩形上 。这个矩形贴满整个窗口。
	glBegin(GL_QUADS);
		glTexCoord2f(0 ,0);	    glVertex3f(-1,-1,-5);
		glTexCoord2f(tx,0);	    glVertex3f( 1,-1,-5);
		glTexCoord2f(tx,ty);	glVertex3f( 1, 1,-5);
		glTexCoord2f(0 ,ty);	glVertex3f(-1, 1,-5);
	glEnd();


	//混合完毕，我们把混合结果保存到纹理中。
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0,0,0,0,0,m_view_port_x,m_view_port_y);

	if(g_GL.m_view_height > m_view_port_y || g_GL.m_view_width > m_view_port_y)
	{
		
		//现在我们恢复原来的视口。准备画出最后结果
		
		glViewport(0,0,g_GL.m_view_width,g_GL.m_view_height);  
		
		//让这个矩形占满整个视口。并且把最后的结果当作纹理来贴到矩形上。
		//这样我们在屏幕上就只看到这个矩形的 纹理。也就是我们的结果
		glDisable(GL_BLEND);


		glBegin(GL_QUADS);
			glTexCoord2f(0 ,0);	    glVertex3f(-1,-1,-5);
			glTexCoord2f(tx,0);	    glVertex3f( 1,-1,-5);
			glTexCoord2f(tx,ty);	glVertex3f( 1, 1,-5);
			glTexCoord2f(0 ,ty);	glVertex3f(-1, 1,-5);
		glEnd();
	}

	//Draw all the other Effect,
	int eff_n = m_Effects.size();    
   	for(int i=0;i < eff_n ;i++)
	{
		m_Effects[i]->Render(m_tex,tx,ty,m_view_port_x,m_view_port_y);
	}

   //reset the enviroment
   AfterRender();
}

void CMotionBlur::PreRender()
{
//首先我们设置平面的投影方式。我们现在将要进行平面的图象混合操作。
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1,1.0,-1,1.0,-100,100);
    
	//单位的视图矩阵。我们不需要任何的变换，因为是平面的图象操作。
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	//打开纹理和混合功能。
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,m_tex);

	glDisable(GL_DEPTH_TEST);
		

	//我们要把这个多边形的模式设置为实体模式。不然我们就不能显示线框模式的
	//场景的运动模糊
	glPushAttrib(GL_POLYGON_BIT); 
	glPolygonMode(GL_FRONT,GL_FILL);
	glPolygonMode(GL_BACK,GL_FILL);
}

void CMotionBlur::AfterRender()
{
	//恢复投影矩阵和视图矩阵。
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glPopAttrib();
}
void CMotionBlur::Create(int size_x,int size_y)
{


	if( m_bIsCreated == TRUE)
		return ;
  
	m_bIsCreated = TRUE;

	for(int i=2;1,i*=2;)
	{
		if(size_x > i/2 && size_x <=i)
		{
			m_tex_size_x = i;
			break;
		}
	}

	for(int j = 2;1;j*=2)
	{
		if(size_y > j/2 && size_y <=j)
		{
			m_tex_size_y = j;
			break;
		}
	}


	unsigned char *pTexture = NULL;											

	pTexture = new unsigned char [ j * i * 3];
	memset(pTexture, 1.0, i * j * 3 * sizeof(unsigned char));	

	glGenTextures(1, &m_tex);								
	glBindTexture(GL_TEXTURE_2D, m_tex);					
	
	glTexImage2D(GL_TEXTURE_2D, 0, 3, i, j, 0, GL_RGB,GL_UNSIGNED_BYTE, pTexture);						
	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
	glEnable(GL_TEXTURE_2D);

	delete [] pTexture;		
}

void CMotionBlur::Resize()
{
	m_view_port_x = g_GL.m_view_width;
	m_view_port_y = g_GL.m_view_height;
	
	if(m_view_port_x >0 && m_view_port_y>0)
	{
		Create(m_view_port_x,m_view_port_y);
	}

	if(m_view_port_x > m_tex_size_x || m_view_port_y > m_tex_size_y)
	{
		if( (float)m_view_port_x /m_view_port_y > (float)m_tex_size_x/m_tex_size_y)
		{
			float f=(float)m_view_port_y /m_view_port_x;
			m_view_port_x = m_tex_size_x;
			m_view_port_y =  f * m_tex_size_x;
		}
		else
		{
			float f = (float)m_view_port_x /m_view_port_y;
			m_view_port_y = m_tex_size_y;
			m_view_port_x = f * m_tex_size_y;
		}
	}

}

void CMotionBlur::SetEffected(BOOL eflag)
{
	m_bEffected = eflag;
}

BOOL CMotionBlur::IsEffected()
{
	return m_bEffected;
}

void CMotionBlur::SetBlurValue(float f)
{
	if(f>0.5 && f<0.87)
		m_blur_val = f;
}

float CMotionBlur::GetBlurValue()
{
	return m_blur_val;
}



void CMotionBlur::OnlyRenderOtherEffect()
{
       //在这个函数里面。我们要渲染所有的效果。
	   //但是纹理中并没有保存着我们先前的渲染结果。
	   //先检查所有的效果，如果有开启的效果，就直接返回，
	   //否则要 从帧缓冲 拷贝图象到纹理。
	   int i;
	   int eff_n = m_Effects.size();
	   BOOL needCopy = FALSE;
	   BOOL isEff = FALSE;
       for(i=0;i< eff_n ;i++)
	   {
		   if(m_Effects[i]->IsNeedFrameTex())
			   needCopy = TRUE;
		   if(m_Effects[i]->IsEffect())
			   isEff = TRUE;
	   }

	
	   if(!isEff)
		   return;

	   //纹理坐标的区域，即最大纹理坐标
	   	float tx = m_view_port_x/(float)m_tex_size_x;
	    float ty = m_view_port_y/(float)m_tex_size_y;

		//设置环境。
		PreRender();

		if(g_GL.m_view_height > m_view_port_y || g_GL.m_view_width > m_view_port_y)
		{
			
			//现在我们恢复原来的视口。准备画出最后结果
			
			glViewport(0,0,g_GL.m_view_width,g_GL.m_view_height);  
		}
		//拷贝图象。
		if(needCopy)
			glCopyTexSubImage2D(GL_TEXTURE_2D, 0,0,0,0,0,m_view_port_x,m_view_port_y);
      
		//渲染效果。
		for(i=0;i < eff_n ;i++)
		{
		   m_Effects[i]->Render(m_tex,tx,ty,m_view_port_x,m_view_port_y);
		}
       
		AfterRender();
		return;
}

int CMotionBlur::AddEffect(CEffect *eff)
{
	int n = m_Effects.size();
    m_Effects.push_back(eff);
	return n;
}

BOOL CMotionBlur::RemoveEffect(CEffect *eff)
{
	vector<CEffect*>::iterator i;
	for(i=m_Effects.begin();i<m_Effects.end();i++)
	{
		if( (*i) == eff)
		{
			m_Effects.erase(i);
			return TRUE;
		}
	}
	return FALSE;
}

void CMotionBlur::Render(PFNCRENDERBLUR pfnRender,long fps)
{
	BeginRender();
	pfnRender(fps);
	Render();
}
