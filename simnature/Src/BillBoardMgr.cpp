/***************************************************************
   ģ������:
       BillBoardMgr.cpp
   Ŀ��: 
      �ṩ��BillBoardϵͳ��������֧��;

   ����:
        ������ HeartBlue
		       XpertSoft HeartBlue 2002/9/29
			   Allright reserved

  Ŀ�ģ�
     ����֪������OpenGL���л���������Ҫ�����ġ�
	 ���ǰ���ͬ�����BillBoard�ŵ���ͬ���������棬
	 ������Ⱦ��ͬ������Ĺ���壬
	 ͬʱ�����ǰ���ת�ļ���ŵ�BillBoard Manager�
	 ����������ֻҪ����һ�νǶȾͿ����ˡ�
***************************************************************/
#include "BillBoard.h"
#include "BillBoardMgr.h"
#include "ViewFrustum.h"
C2DBillBoardMgr::C2DBillBoardMgr()
{
   m_Culler = NULL;
}
C2DBillBoardMgr::~C2DBillBoardMgr()
{
	//������е�����������ɾ�����еĹ����
	for(int k = 0;k<m_Container.size();k++)
	{
		int n = m_Container[k]._BillBoards.size();
		for(int i=0;i<n;i++)
		{
			delete m_Container[k]._BillBoards[i];
		}
	}
	m_Container.clear();
}
//==================================================================
//����������������λ�ã�����һ��BillBoard��������Ӧ�������С�
//������
//        pos : λ�ã�
//        width,heigbt:��͸ߣ�Ӱ��BillBoard�Ĵ�С
//        texID:����ġ����ָ��������������ʹ���������
//���أ�ָ�򴴽��õ�BillBoard��ָ��
//===================================================================
C2DBillBoard* C2DBillBoardMgr::CreateBillBoard(MVECTOR pos,int width,int height,GLuint texID)
{
	C2DBillBoard* pb = new C2DBillBoard;
	pb->m_height = height;
	pb->m_width  = width;
	pb->m_x = pos.x;
	pb->m_y = pos.y;
	pb->m_z = pos.z;

	//����ƥ������� ��������
	int cn = m_Container.size();
	for(int i=0;i<cn;i++)
	{
		if(texID == m_Container[i]._texID)
		{
			m_Container[i]._BillBoards.push_back(pb);
			return pb;
		}
	}

	//�Ҳ���ƥ�䣬�ʹ���һ������
	bb_Container c;
	c._texID = texID;
	
	m_Container.push_back(c);
	m_Container[cn]._BillBoards.push_back(pb);
	return pb;
	
}

//=========================================================
//������Ǻ���Ӱ����صġ��������Ϊ�����ָ��һ��Camera
//pcm: �۲��ߵ�Camera
//========================================================
void C2DBillBoardMgr::SetCameraInfo(CCamera* pcm)
{
	m_cinfo= pcm->GetCameraInfo();
}
//========================================================
//�������еĹ����
//========================================================
void C2DBillBoardMgr::Draw()
{

	MVECTOR dir,vup;

	if (m_cinfo == NULL)
		return ;
	//��Ӱ���ķ�������������ȡ��������ֻ�õ�ƽ�����������
	dir.x=  -m_cinfo->vdir.x;
	dir.y=  -m_cinfo->vdir.y;
	dir.z=  0;
    Math.Normalize(dir); 

	if(dir.x == 0 && dir.y == 0)
		return ;
	//������Ӱ���ķ����BillBoardԭʼ����ļнǡ�	
	int theta = Math.GetVectorClamp(m_vDir,dir);  
	//���������Ҫ������ת���ᣨ���������ܵķ�����ת�պ��෴��
	Math.Cross(m_vDir,dir,vup);
 
	for(int c_i = 0;c_i<m_Container.size();c_i++)
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);

		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_EQUAL,1.); 

		glBindTexture(GL_TEXTURE_2D,m_Container[c_i]._texID);
		glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
		
		int n=m_Container[c_i]._BillBoards.size();
		C2DBillBoard* pb;

		if(m_Culler==NULL)
		{
			for(int i=0;i<n;i++)
			{
				pb= m_Container[c_i]._BillBoards[i];
				pb->Draw(theta,vup);
			}
		}
		else
		{
				for(int i=0;i<n;i++)
				{
					pb= m_Container[c_i]._BillBoards[i];
					if(! m_Culler->IsPtCulled(pb->m_x,pb->m_y,pb->m_z))    
			 				 pb->Draw(theta,vup);
				}
		}
	}//End for c_i
    
	glDisable(GL_ALPHA_TEST);
}

//=====================================================
//��������ţ�����һ�������/
//����
//       bb     �����õĹ����
//       index  ������š�ע�⣬����ţ���������
//�������ų���������Ŀ������ֱ�ӷ��ء�
//=====================================================
void C2DBillBoardMgr::Add(C2DBillBoard* bb,int index)
{
	if(index > m_Container.size())
		return;
	m_Container[index]._BillBoards.push_back(bb); 
}
//=====================================================
//����һ����������һ��������
//����
//       texID�� ����
//     ����������Ѿ����ڣ��򲻴���
//���ذ����������������ı�š�
//=====================================================
int C2DBillBoardMgr::CreateContainer(GLuint texID)
{
	int cn = m_Container.size();
	for(int i=0;i<cn;i++)
	{
		if(texID == m_Container[i]._texID)
		{
			//�Ѿ����ڣ���ʲô���������򵥵ķ���
			return i;
		}
	}
   
	//����һ������
	bb_Container c;
	c._texID = texID;
	m_Container.push_back(c);
	return cn;
}

int C2DBillBoardMgr::GetTypeCount()
{
	return m_Container.size();
}
