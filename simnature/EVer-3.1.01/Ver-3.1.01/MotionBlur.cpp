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
	//��������ʹ�û�Ϲ��ܣ�����ǰ����������ͼ��͵�ǰ����Ⱦ������������
	glColor4f(1.0,1.0,1.0,m_blur_val);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//�������ǵ�����ͼ��Ĵ�С�� 1/1�����Ǵ��ڵı���ȴ���ǡ�
	//��������Ҫ�г�û����Ⱦ�Ĳ��֣����ڵ��ǲ��֣�
	float tx = m_view_port_x/(float)m_tex_size_x;
	float ty = m_view_port_y/(float)m_tex_size_y;

	//����ǰͼ����Ϊһ��������һ�������� ��������������������ڡ�
	glBegin(GL_QUADS);
		glTexCoord2f(0 ,0);	    glVertex3f(-1,-1,-5);
		glTexCoord2f(tx,0);	    glVertex3f( 1,-1,-5);
		glTexCoord2f(tx,ty);	glVertex3f( 1, 1,-5);
		glTexCoord2f(0 ,ty);	glVertex3f(-1, 1,-5);
	glEnd();


	//�����ϣ����ǰѻ�Ͻ�����浽�����С�
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0,0,0,0,0,m_view_port_x,m_view_port_y);

	if(g_GL.m_view_height > m_view_port_y || g_GL.m_view_width > m_view_port_y)
	{
		
		//�������ǻָ�ԭ�����ӿڡ�׼�����������
		
		glViewport(0,0,g_GL.m_view_width,g_GL.m_view_height);  
		
		//���������ռ�������ӿڡ����Ұ����Ľ���������������������ϡ�
		//������������Ļ�Ͼ�ֻ����������ε� ����Ҳ�������ǵĽ��
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
//������������ƽ���ͶӰ��ʽ���������ڽ�Ҫ����ƽ���ͼ���ϲ�����
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1,1.0,-1,1.0,-100,100);
    
	//��λ����ͼ�������ǲ���Ҫ�κεı任����Ϊ��ƽ���ͼ�������
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	//������ͻ�Ϲ��ܡ�
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,m_tex);

	glDisable(GL_DEPTH_TEST);
		

	//����Ҫ���������ε�ģʽ����Ϊʵ��ģʽ����Ȼ���ǾͲ�����ʾ�߿�ģʽ��
	//�������˶�ģ��
	glPushAttrib(GL_POLYGON_BIT); 
	glPolygonMode(GL_FRONT,GL_FILL);
	glPolygonMode(GL_BACK,GL_FILL);
}

void CMotionBlur::AfterRender()
{
	//�ָ�ͶӰ�������ͼ����
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
       //������������档����Ҫ��Ⱦ���е�Ч����
	   //���������в�û�б�����������ǰ����Ⱦ�����
	   //�ȼ�����е�Ч��������п�����Ч������ֱ�ӷ��أ�
	   //����Ҫ ��֡���� ����ͼ������
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

	   //������������򣬼������������
	   	float tx = m_view_port_x/(float)m_tex_size_x;
	    float ty = m_view_port_y/(float)m_tex_size_y;

		//���û�����
		PreRender();

		if(g_GL.m_view_height > m_view_port_y || g_GL.m_view_width > m_view_port_y)
		{
			
			//�������ǻָ�ԭ�����ӿڡ�׼�����������
			
			glViewport(0,0,g_GL.m_view_width,g_GL.m_view_height);  
		}
		//����ͼ��
		if(needCopy)
			glCopyTexSubImage2D(GL_TEXTURE_2D, 0,0,0,0,0,m_view_port_x,m_view_port_y);
      
		//��ȾЧ����
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
