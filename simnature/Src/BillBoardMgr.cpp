/***************************************************************
   模块名称:
       BillBoardMgr.cpp
   目的: 
      提供对BillBoard系统分类管理的支持;

   作者:
        潘李亮 HeartBlue
		       XpertSoft HeartBlue 2002/9/29
			   Allright reserved

  目的：
     我们知道，在OpenGL里切换纹理是需要开销的。
	 我们把相同纹理的BillBoard放到相同的容器里面，
	 依次渲染相同的纹理的公告板，
	 同时，我们把旋转的计算放到BillBoard Manager里。
	 这样，我们只要计算一次角度就可以了。
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
	//清空所有的容器。并且删除所有的公告板
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
//按给定的纹理。长宽，位置，创建一个BillBoard。并且相应的容器中。
//参数：
//        pos : 位置，
//        width,heigbt:宽和高，影响BillBoard的大小
//        texID:纹理的。如果指定纹理不在容器里。就创建容器。
//返回：指向创建好的BillBoard的指针
//===================================================================
C2DBillBoard* C2DBillBoardMgr::CreateBillBoard(MVECTOR pos,int width,int height,GLuint texID)
{
	C2DBillBoard* pb = new C2DBillBoard;
	pb->m_height = height;
	pb->m_width  = width;
	pb->m_x = pos.x;
	pb->m_y = pos.y;
	pb->m_z = pos.z;

	//搜索匹配的纹理 的容器。
	int cn = m_Container.size();
	for(int i=0;i<cn;i++)
	{
		if(texID == m_Container[i]._texID)
		{
			m_Container[i]._BillBoards.push_back(pb);
			return pb;
		}
	}

	//找不到匹配，就创建一个容器
	bb_Container c;
	c._texID = texID;
	
	m_Container.push_back(c);
	m_Container[cn]._BillBoards.push_back(pb);
	return pb;
	
}

//=========================================================
//公告板是和摄影机相关的。这个函数为公告板指定一个Camera
//pcm: 观察者的Camera
//========================================================
void C2DBillBoardMgr::SetCameraInfo(CCamera* pcm)
{
	m_cinfo= pcm->GetCameraInfo();
}
//========================================================
//绘制所有的公告板
//========================================================
void C2DBillBoardMgr::Draw()
{

	MVECTOR dir,vup;

	if (m_cinfo == NULL)
		return ;
	//摄影机的方向，在这里我们取反。而且只用到平面的两个方向。
	dir.x=  -m_cinfo->vdir.x;
	dir.y=  -m_cinfo->vdir.y;
	dir.z=  0;
    Math.Normalize(dir); 

	if(dir.x == 0 && dir.y == 0)
		return ;
	//计算摄影机的方向和BillBoard原始朝向的夹角。	
	int theta = Math.GetVectorClamp(m_vDir,dir);  
	//计算出我们要绕着旋转的轴（有两个可能的方向，旋转刚好相反）
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
//按容器编号，加入一个公告板/
//参数
//       bb     创建好的公告板
//       index  容器编号。注意，是序号，不是纹理。
//如果，编号超过容器数目，函数直接返回。
//=====================================================
void C2DBillBoardMgr::Add(C2DBillBoard* bb,int index)
{
	if(index > m_Container.size())
		return;
	m_Container[index]._BillBoards.push_back(bb); 
}
//=====================================================
//给定一个纹理，创建一个容器。
//参数
//       texID： 纹理。
//     如果，容器已经存在，则不创建
//返回包含这个纹理的容器的编号。
//=====================================================
int C2DBillBoardMgr::CreateContainer(GLuint texID)
{
	int cn = m_Container.size();
	for(int i=0;i<cn;i++)
	{
		if(texID == m_Container[i]._texID)
		{
			//已经存在，则什么都不做，简单的返回
			return i;
		}
	}
   
	//创建一个容器
	bb_Container c;
	c._texID = texID;
	m_Container.push_back(c);
	return cn;
}

int C2DBillBoardMgr::GetTypeCount()
{
	return m_Container.size();
}
