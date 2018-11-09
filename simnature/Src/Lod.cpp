#include <windows.h>
#include <gl\gl.h>
#include <math.h>
#include "Terrain.h"
#include "LOD.h"
#include "texturemgr.h"

float Max8(float e[])
{
	float max_e = e[0];
	for(int i =1;i<8;i++)
	{
		if(e[i]>max_e)
			max_e = e[i];
	}
	return max_e;
}
float E2(int h1,int h2,int oh,int l)
{
	return abs( (h1 + h2)/2.0 - oh  ) /2.0; 
}
void SetMultiTexCoord(float u, float v)
{
	//	glTexCoord2f(u,v);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, u, v);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB, u, v);
}
void SetSingleTexCoord(float u, float v)
{
	glTexCoord2f(u,v);
}

VERTEX_STATE  CLOD::CanActive(int x,int y,int size)
{
   float dx = x - m_view_x;
   float dy = y - m_view_y;
   float z=m_pTerrain->GetRawValue(x,y);
   float dz = z - m_view_z;
   float dist = dx * dx + dy * dy + dz * dz;



 //  被注释掉的代码原来是用来计算Pixel Error的，现在没有用
 //  double win_x,win_y,win_z;
 //  gluProject(x,y,m_variant[x + m_w * y],m_mv,m_mp,m_vp,&win_x,&win_y,&win_z);
   
   float f = m_error * size * m_variant.GetValue(x,y)/ (6.0*dist);
   if(f <1)
	   return VS_DISABLE;
   else
	   return VS_ACTIVE;
  

  //这里的判断公式决定 Level的细节。
  if(m_variant.GetValue(x,y) > m_object_error)
 	  return VS_ACTIVE;

  if(m_variant.GetValue(x,y)  == 0)
 	  return VS_DISABLE;

  if(dist  <= m_space_error * (size * size))
		return VS_ACTIVE;
	return VS_DISABLE;

}
#define INDEX(x,y) ( (x) + m_h*(y) )

void  CLOD::InitLODInfo()
{
	//m_delta数组里保存着每一个对应的level的 面片的大小。
	//在 CLOD类的构造的时候，要把这个数组给初始化好。
	int size = m_w - 1; 
	m_maxlevel = -2;
	for(int i=0;size;i++)
	{
		this->m_delta[i] = size/2;
		size = size/2;
	    m_maxlevel++;
	}

}

#define GetDelta(x) m_delta[(x)]
//==================================================================
//该函数为 LOD 面片更新函数的入口。我们首先把整个地形给激活。
//我们一次一个 层次(level)的测试每个节点，如果需要激活它的的子节点的话
//就把把它的子节点推到一个队列中，以备在下一个层次测试的时候用。
//如果一个节点没有子节点要要激活，我们就 Disable掉它的四个子节点，以防止产生裂缝
//同时画出这个节点。
//如果到达完全分辨率的时候，我们也简单的画出来。
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void  CLOD::RenderLOD()
{
	
#ifdef _COUNT_TRIAN_COUNT
			m_trian_count = 0;
#endif
   	vector<QNODE>  v1;
	vector<QNODE>  v2;
	vector<QNODE>* cur_Queue;
	vector<QNODE>* next_Queue,*temp_v;
	cur_Queue =&v1;next_Queue = &v2;

	//First . Push the root node to the cur_Queue;
	int level = 0,i=0;
	int d = m_delta[0];
	int cx = (m_w - 1)/2.;
	int cy = (m_h - 1)/2.;
    QNODE node ={cx,cy};
	cur_Queue->push_back(node);
	while( d > 0)
	{
		int vlen = cur_Queue->size();
		for(i = 0 ; i < vlen;i++)
		{
			node = (*cur_Queue)[i];
			cx = node._x;
			cy = node._y;
			if(m_cull->IsRectCulled(cx,cy,d*2))
			{
			    DividNode(cx,cy,d);
				continue;
			}
			//Get Neighbor node information
			CheckNeighbor(cx,cy,d);
			if(NodeCanDivid(cx,cy,d) && CanActive(cx,cy,2 * d) )
			{
				if( d == 1)
				{
 					m_active.GetValue(cx,cy)=VS_ACTIVE;
					DrawPrim(cx,cy);
				}
				else
				{
					DividNode(cx,cy,d);
					//Push the left_up sub node to the next level Queue
					node._x = cx - d/2;
					node._y = cy + d/2;
					next_Queue->push_back(node);

					//Push the left_down sub node to the next level queue
					node._x = cx - d/2;
					node._y = cy - d/2;
					next_Queue->push_back(node);

					//Push the right_up sub node to the next level queue
					node._x = cx + d/2;
					node._y = cy + d/2;
					next_Queue->push_back(node);

					//Push the right_down sub node to the next level queue
					node._x = cx + d/2;
					node._y = cy - d/2;
					next_Queue->push_back(node);
				}

			}
			else
			{
				DisableNode(cx,cy,d);
				DrawNode(cx,cy,d);
			}
			
		}//end for
		cur_Queue->clear();

		//Swap the two queue
		temp_v = cur_Queue;
		cur_Queue = next_Queue;
		next_Queue = temp_v;
		level ++ ;
		d = m_delta[level];
	}//end while
}
BOOL  CLOD::Render(int* texID)
{
  
	//Befor render we first prepair the errror metrics (Just for Speed)
	m_error = m_object_error * m_space_error; 
	//Now setup the camera position and prepair update the adaptive mesh
	SetViewer();
	//Prepaire the texture map setting
	PreRender();
	//Recure draw all the quad-tree node
	if(m_lodmethod%2)
		RenderLOD2();
	else
		RenderLOD();
	//reset the texture map enviroment (Shutdown multi-texture map)
   	AfterRender(); 

	return TRUE;
}
void CLOD::SetViewer()
{
	MVECTOR m = m_cull->GetViewerPos(); 
	m_view_x = m.x;
	m_view_y = m.y;
	m_view_z = m.z;

}

CLOD::CLOD(CTerrain* t)
{
    m_pTerrain=t;
	m_mesh_size = t->GetX();
	m_w = t->m_w;
	m_h = t->m_h;
//	m_variant = NULL;
	m_active.Create(t->m_w,t->m_h);//  = new VERTEX_STATE [t->m_h * t->m_w ];
	m_variant.Create(t->m_w,t->m_h);//      = new float [t->m_h * t->m_w ];

	memset((void*)m_active.m_pages,VS_UNREACH,m_active.GetSize());
	InitLODInfo(); 
	m_cull = NULL;
	m_is_Detail_tex = FALSE;
	m_is_Grid       = FALSE;
	SetTexCoord = SetSingleTexCoord;
	m_tex_Detail = 30;
	m_lodmethod  = 0;


}

bool CLOD::IsViewerInRect(int cx, int cy, int d)
{
	int d1 = cx-m_view_x;
	int d2 = cy-m_view_y;
	if(abs(d1)<d&&abs(d2)<d)
		return true;
	return false;
}

void CLOD::PreRender()
{
	if(!m_is_Grid)
	{
		if(m_is_Detail_tex) 
		{
			
			glActiveTextureARB(GL_TEXTURE0_ARB);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, m_pTerrain->m_texID[0]);
			
			glActiveTextureARB(GL_TEXTURE1_ARB);
			glEnable(GL_TEXTURE_2D);
			
			// Here we turn on the COMBINE properties and increase our RGB
			// gamma for the detail texture.  2 seems to work just right.
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
			glTexEnvi(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 2);
			
			// Bind the detail texture
			glBindTexture(GL_TEXTURE_2D, m_pTerrain->m_texID[m_dtex]);
			
			
			glMatrixMode(GL_TEXTURE);
			
			//用纹理矩阵对细节纹理进行放大。
			glLoadIdentity();
			glScalef((float)m_tex_Detail, (float)m_tex_Detail, 1);
			
			// Leave the texture matrix and set us back in the model view matrix
			glMatrixMode(GL_MODELVIEW);
			
		}
		else
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,m_pTerrain->m_texID[0]);
			glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
			glColor4f(1,1,1,1);
		}
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}
}

void CLOD::AfterRender()
{
	if(m_is_Detail_tex) 
	{
		// Turn the second multitexture pass off
		glActiveTextureARB(GL_TEXTURE1_ARB);
		glDisable(GL_TEXTURE_2D);
		
		// Turn the first multitexture pass off
		glActiveTextureARB(GL_TEXTURE0_ARB);		
		glDisable(GL_TEXTURE_2D);
	}
}

void CLOD::EnableDetailTex(BOOL b)
{
	if(b)
	{
		SetTexCoord = SetMultiTexCoord;
		m_is_Detail_tex = TRUE;
	}
	else
	{
		SetTexCoord = SetSingleTexCoord;
		m_is_Detail_tex = FALSE;
	}
}

void CLOD::SetTexDetail(float detail)
{
	m_tex_Detail = detail;
}

void CLOD::IncreaseTexDetail(float delta)
{
	m_tex_Detail += delta;
}

#define E_LEFT       0
#define E_RIGHT      1
#define E_UP         2
#define E_DOWN       3 
#define E_CENTER     4
#define E_LEFT_UP    5
#define E_LEFT_DOWN  6
#define E_RIGHT_UP   7
#define E_RIGHT_DOWN 8

void CLOD::BuildVarMap()
{
   int cx = (m_w-1)/2;
   int cy = (m_h-1)/2;
   int level = 0;
 
   VarMax(cx,cy,level);

}

float CLOD::VarMax(int cx, int cy, int level)
{
   int d = m_delta[level];
   float e[9]={0};

   if(d >1)
   {
	   e[E_LEFT_UP   ]  = VarMax(cx - d/2,cy + d/2,level + 1);
	   e[E_LEFT_DOWN ]  = VarMax(cx - d/2,cy - d/2,level + 1);
	   e[E_RIGHT_UP  ]  = VarMax(cx + d/2,cy + d/2,level + 1);
	   e[E_RIGHT_DOWN]  = VarMax(cx + d/2,cy - d/2,level + 1);
   }

   e[E_LEFT]  = E2(m_pTerrain->GetRawValue(cx - d,cy + d),
	               m_pTerrain->GetRawValue(cx - d,cy - d),
				   m_pTerrain->GetRawValue(cx - d,cy    ), 2 * d);
   m_variant.GetValue(cx-d,cy) = e[E_LEFT];

   e[E_UP]    = E2(m_pTerrain->GetRawValue(cx - d,cy + d),
	               m_pTerrain->GetRawValue(cx + d,cy + d),
				   m_pTerrain->GetRawValue(cx    ,cy + d), 2 * d);
   m_variant.GetValue(cx,cy + d) = e[E_UP];

   e[E_RIGHT] = E2(m_pTerrain->GetRawValue(cx + d,cy + d),
	               m_pTerrain->GetRawValue(cx + d,cy - d),
				   m_pTerrain->GetRawValue(cx + d,cy    ), 2 * d);
   m_variant.GetValue(cx + d,cy ) = e[E_RIGHT];

   e[E_DOWN]  = E2(m_pTerrain->GetRawValue(cx + d,cy - d),
	               m_pTerrain->GetRawValue(cx - d,cy - d),
				   m_pTerrain->GetRawValue(cx    ,cy - d), 2 * d);
   m_variant.GetValue(cx , cy - d) = e[E_DOWN];

   e[E_CENTER] =E2(m_pTerrain->GetRawValue(cx ,cy-d),
	               m_pTerrain->GetRawValue(cx ,cy+d),
				   m_pTerrain->GetRawValue(cx ,cy), 2*d); 

  m_variant.SetValue(cx,cy, Max8(e));
  return m_variant.GetValue(cx,cy);
   
}

#undef E_LEFT       
#undef E_RIGHT      
#undef E_UP         
#undef E_DOWN        
#undef E_CENTER     
#undef E_LEFT_UP    
#undef E_LEFT_DOWN  
#undef E_RIGHT_UP   
#undef E_RIGHT_DOWN 

void CLOD::SetObjectError(float erro)
{
   m_object_error = erro * erro;
}
float CLOD::GetObjectError()
{
	return sqrt(m_object_error);
}

void CLOD::ChangeDetailTex(GLuint tex)
{
	m_dtex = tex %(m_pTerrain->m_texNum -1) + 1; 
}

void CLOD::DisableNode(int cx, int cy, int d)
{
	int d2 = d/2;
	m_active.GetValue(cx,cy) = VS_ACTIVE;

	if(d == 1)
		return;
	m_active.SetValue( cx - d2, cy - d2, VS_DISABLE);
	m_active.SetValue( cx - d2, cy + d2, VS_DISABLE);
	m_active.SetValue( cx + d2, cy - d2, VS_DISABLE);
	m_active.SetValue( cx + d2, cy + d2, VS_DISABLE);
}

void CLOD::DividNode(int cx, int cy, int d)
{
	int d2 = d/2;
	m_active.GetValue(cx,cy) = VS_ACTIVE;

	m_active.SetValue( cx - d2, cy - d2, VS_ACTIVE);
	m_active.SetValue( cx - d2, cy + d2, VS_ACTIVE);
	m_active.SetValue( cx + d2, cy - d2, VS_ACTIVE);
	m_active.SetValue( cx + d2, cy + d2, VS_ACTIVE);
}

BOOL CLOD::NodeCanDivid(int cx, int cy, int d)
{

	
    if(m_neighbor[0] == VS_DISABLE)
	   return FALSE;
	if(m_neighbor[1] == VS_DISABLE)
	   return FALSE;
	if(m_neighbor[2] == VS_DISABLE)
	   return FALSE;
	if(m_neighbor[3] == VS_DISABLE)
	   return FALSE;
	return TRUE;
}

void CLOD::DrawPrim(int cx, int cy)
{
	float tcx = float(cx)/m_w;
	float tcy = float(cy)/m_h;
	float tcd = 1.0/m_h;

#ifdef _COUNT_TRIAN_COUNT
			m_trian_count += 8;
#endif
	glBegin(GL_TRIANGLE_FAN);
	    //Center
		glColor3f(0.0,1,0);
		SetTexCoord(tcx,tcy);
		glVertex3f(cx,cy,m_pTerrain->GetValue(cx,cy));

		glColor3f(1.0,1,1);
		//Left_up
		SetTexCoord(tcx - tcd, tcy + tcd);
		glVertex3f(cx - 1,cy + 1,m_pTerrain->GetValue(cx - 1,cy + 1));

		//Left
			SetTexCoord(tcx - tcd, tcy);
			glVertex3f(cx - 1,cy,m_pTerrain->GetValue(cx - 1,cy));

		//Left down
		SetTexCoord(tcx - tcd, tcy - tcd);
		glVertex3f(cx - 1,cy - 1,m_pTerrain->GetValue(cx - 1,cy - 1));

		//down

			SetTexCoord(tcx , tcy - tcd);
			glVertex3f(cx,cy - 1,m_pTerrain->GetValue(cx,cy - 1));

		//right down
		SetTexCoord(tcx + tcd, tcy - tcd);
		glVertex3f(cx + 1,cy - 1,m_pTerrain->GetValue(cx + 1,cy - 1));

		//right

			SetTexCoord(tcx + tcd, tcy);
			glVertex3f(cx + 1,cy,m_pTerrain->GetValue(cx + 1,cy));

		//Right up
		SetTexCoord(tcx + tcd, tcy + tcd);
		glVertex3f(cx + 1,cy + 1,m_pTerrain->GetValue(cx + 1,cy + 1));

		//up

			SetTexCoord(tcx , tcy + tcd);
			glVertex3f(cx,cy + 1,m_pTerrain->GetValue(cx,cy + 1));

		//Left_up
		SetTexCoord(tcx - tcd, tcy + tcd);
		glVertex3f(cx - 1,cy + 1,m_pTerrain->GetValue(cx - 1,cy + 1));

	glEnd();
}

void CLOD::DrawNode(int cx, int cy, int d)
{
	float tcx = float(cx)/m_w;
	float tcy = float(cy)/m_h;
	float tcd = float(d)/m_h;
	int d2 = d*2;

	
#ifdef _COUNT_TRIAN_COUNT
			m_trian_count += 4;
#endif
	glBegin(GL_TRIANGLE_FAN);
	    //Center
		glColor3f(1.0,0,0);
		if(d == 1)
			glColor4f(0,0,1.0,0);
		SetTexCoord(tcx,tcy);
		glVertex3f(cx,cy,m_pTerrain->GetValue(cx,cy));

		glColor3f(1.0,1,1);
		//Left_up
		SetTexCoord(tcx - tcd, tcy + tcd);
		glVertex3f(cx - d,cy + d,m_pTerrain->GetValue(cx - d,cy + d));

		//Left
		//nx = cx - d2;ny=cy;
		if(m_neighbor[NV_LEFT] == VS_ACTIVE)//nx < 0 || m_active.GetValue(nx,ny) != VS_DISABLE)
		{
			
#ifdef _COUNT_TRIAN_COUNT
			m_trian_count++;
#endif
			SetTexCoord(tcx - tcd, tcy);
			glVertex3f(cx - d,cy,m_pTerrain->GetValue(cx - d,cy));
		}

		//Left down
		SetTexCoord(tcx - tcd, tcy - tcd);
		glVertex3f(cx - d,cy - d,m_pTerrain->GetValue(cx - d,cy - d));

		//down
		//nx = cx; ny = cy - d2;
		if(m_neighbor[NV_DOWN] == VS_ACTIVE)//ny < 0 || m_active.GetValue(nx,ny) != VS_DISABLE)
		{

#ifdef _COUNT_TRIAN_COUNT
			m_trian_count++;
#endif
			SetTexCoord(tcx , tcy - tcd);
			glVertex3f(cx,cy - d,m_pTerrain->GetValue(cx,cy - d));
		}

		//right down
		SetTexCoord(tcx + tcd, tcy - tcd);
		glVertex3f(cx + d,cy - d,m_pTerrain->GetValue(cx + d,cy - d));

		//right
		//nx = cx + d2; ny = cy;
		if(m_neighbor[NV_RIGHT] == VS_ACTIVE)//ny >= m_w || m_active.GetValue(nx,ny) != VS_DISABLE)
		{
			
#ifdef _COUNT_TRIAN_COUNT
			m_trian_count++;
#endif
			SetTexCoord(tcx + tcd, tcy);
			glVertex3f(cx + d,cy,m_pTerrain->GetValue(cx + d,cy));
		}

		//Right up
		SetTexCoord(tcx + tcd, tcy + tcd);
		glVertex3f(cx + d,cy + d,m_pTerrain->GetValue(cx + d,cy + d));

		//up
		//nx = cx; ny = cy + d2;
		if(m_neighbor[NV_UP] == VS_ACTIVE)//ny >= m_h || m_active.GetValue(nx,ny) != VS_DISABLE)
		{
			
#ifdef _COUNT_TRIAN_COUNT
			m_trian_count++;
#endif
			SetTexCoord(tcx , tcy + tcd);
			glVertex3f(cx,cy + d,m_pTerrain->GetValue(cx,cy + d));
		}

		//Left_up
		SetTexCoord(tcx - tcd, tcy + tcd);
		glVertex3f(cx - d,cy + d,m_pTerrain->GetValue(cx - d,cy + d));

	glEnd();
}



void CLOD::RenderLOD2()
{
	
#ifdef _COUNT_TRIAN_COUNT
			m_trian_count = 0;
#endif
   	vector<QNODE>  v1;
	vector<QNODE>  v2;
	vector<QNODE>* cur_Queue;
	vector<QNODE>* next_Queue,*temp_v;
	cur_Queue =&v1;next_Queue = &v2;

	int level = 0,i=0;
	int d = m_delta[0];
	int d2;
	int nx,ny;
	int cx = (m_w - 1)/2.;
	int cy = (m_h - 1)/2.;
    QNODE node ={cx,cy};
	cur_Queue->push_back(node);
	while( d > 0)
	{
		d2 = d/2;
		int vlen = cur_Queue->size();
		for(i = 0 ; i < vlen;i++)
		{
			node = (*cur_Queue)[i];
			cx = node._x;
			cy = node._y;
			
			
			CheckNeighbor(cx,cy,d);

			if(m_cull->IsRectCulled(cx,cy,d*2))
			{
			    DividNode(cx,cy,d);
				continue;
			}
			if( d == 1)
			{
				DrawNode(cx,cy,1);
				continue;
			}

			m_subnode[0] = m_subnode[1] =  m_subnode[2] = m_subnode[3] = FALSE;
			//Left up
			nx = cx - d2;ny=cy + d2;
			if(CanActive(nx,ny,d))
			{
				if(m_neighbor[NV_LEFT] && m_neighbor[NV_UP])
				{
					m_subnode[SN_LU]=TRUE;
					m_active.GetValue(nx,ny) = VS_ACTIVE;
					node._x = nx;node._y=ny;
					next_Queue->push_back(node); 
				}
				else
					m_active.GetValue(nx,ny) = VS_DISABLE;
			}
			else
				m_active.GetValue(nx,ny) = VS_DISABLE;
			//Leftdown
			nx = cx - d2;ny=cy - d2;
			if(CanActive(nx,ny,d))
			{
				if(m_neighbor[NV_LEFT] && m_neighbor[NV_DOWN])
				{
					m_subnode[SN_LD]=TRUE;
					m_active.GetValue(nx,ny) = VS_ACTIVE;
					node._x = nx;node._y=ny;
					next_Queue->push_back(node); 
				}
				else
					m_active.GetValue(nx,ny) = VS_DISABLE;
			}
			else
				m_active.GetValue(nx,ny) = VS_DISABLE;
		    //right dwon
			nx = cx + d2;ny=cy - d2;
			if(CanActive(nx,ny,d))
			{
				if(m_neighbor[NV_RIGHT] && m_neighbor[NV_DOWN])
				{
					m_subnode[SN_RD]=TRUE;
					m_active.GetValue(nx,ny) = VS_ACTIVE;
					node._x = nx;node._y=ny;
					next_Queue->push_back(node); 
				}				
				else
					m_active.GetValue(nx,ny) = VS_DISABLE;
			}
			else
				m_active.GetValue(nx,ny) = VS_DISABLE;
			//right up
			nx = cx + d2;ny=cy + d2;
			if(CanActive(nx,ny,d))
			{
				if(m_neighbor[NV_RIGHT] && m_neighbor[NV_UP])
				{
					m_subnode[SN_RU]=TRUE;
					m_active.GetValue(nx,ny) = VS_ACTIVE;
					node._x = nx;node._y=ny;
					next_Queue->push_back(node); 
				}
				else
					m_active.GetValue(nx,ny) = VS_DISABLE;
			}
			else
				m_active.GetValue(nx,ny) = VS_DISABLE;


			if(m_subnode[0]==FALSE && m_subnode[1]==FALSE&&
				m_subnode[2]==FALSE && m_subnode[3]==FALSE)
			{
				DrawNode(cx,cy,d);
				continue;
			}
			else if(m_subnode[0]==TRUE && m_subnode[1]==TRUE&&
				    m_subnode[2]==TRUE && m_subnode[3]==TRUE)
			{
				continue;
			}
			else
			{
				DrawHalfNode(cx,cy,d);
			}


		}//end for


		cur_Queue->clear();
		temp_v = cur_Queue;
		cur_Queue = next_Queue;
		next_Queue = temp_v;
		level ++ ;
		d = m_delta[level];
	}//end while
}

void CLOD::DrawHalfNode(int cx, int cy, int d)
{
	float tcx = float(cx)/m_w;
	float tcy = float(cy)/m_h;
	float tcd = float(d)/m_h;
	int d2 = d*2;
   //Left up
	if(m_subnode[SN_LU]==FALSE)
	{
		glBegin(GL_TRIANGLE_FAN);
			//Center
			glColor3f(1.0,0,0);
			SetTexCoord(tcx,tcy);
			glVertex3f(cx,cy,m_pTerrain->GetValue(cx,cy));

			glColor3f(1.0,1.0,1.0);
			//up
			if(m_neighbor[NV_UP] == VS_ACTIVE)
			{
				SetTexCoord(tcx,tcy + tcd);
				glVertex3f(cx,cy + d ,m_pTerrain->GetValue(cx,cy + d));
#ifdef _COUNT_TRIAN_COUNT
			m_trian_count ++;
#endif
			}
             
			//Left up
			SetTexCoord(tcx - tcd,tcy + tcd );
			glVertex3f(cx - d,cy + d ,m_pTerrain->GetValue(cx - d,cy + d));

			//Left
			if(m_neighbor[NV_LEFT] == VS_ACTIVE)
			{
				SetTexCoord(tcx - tcd,tcy);
				glVertex3f(cx - d,cy ,m_pTerrain->GetValue(cx - d,cy));
			}
			else
			{
				SetTexCoord(tcx - tcd,tcy - tcd);
				glVertex3f(cx - d,cy - d,m_pTerrain->GetValue(cx - d,cy - d));
			}
 #ifdef _COUNT_TRIAN_COUNT
			m_trian_count ++;
#endif         
		glEnd();
       
	}

	
   //Left DOWN
	if(m_subnode[SN_LD]==FALSE)
	{
		glBegin(GL_TRIANGLE_FAN);
			//Center
			glColor3f(1.0,0,0);
			SetTexCoord(tcx,tcy);
			glVertex3f(cx,cy,m_pTerrain->GetValue(cx,cy));

			glColor3f(1.0,1.0,1.0);
			//left
			if(m_neighbor[NV_LEFT]==VS_ACTIVE)
			{
				SetTexCoord(tcx - tcd,tcy);
				glVertex3f(cx - d,cy ,m_pTerrain->GetValue(cx - d,cy));
#ifdef _COUNT_TRIAN_COUNT
			m_trian_count ++;
#endif
			}
             
			//left down
			SetTexCoord(tcx - tcd,tcy - tcd );
			glVertex3f(cx - d,cy - d ,m_pTerrain->GetValue(cx - d,cy - d));

			//dwon
			if(m_neighbor[NV_DOWN]==VS_ACTIVE)
			{
				SetTexCoord(tcx,tcy - tcd);
				glVertex3f(cx,cy - d ,m_pTerrain->GetValue(cx,cy - d));
			}
			else
			{
				SetTexCoord(tcx + tcd,tcy - tcd);
				glVertex3f(cx + d,cy - d,m_pTerrain->GetValue(cx + d,cy - d));
			}
#ifdef _COUNT_TRIAN_COUNT
			m_trian_count ++;
#endif
		glEnd();
	}

   //RIGHT Down
	if(m_subnode[SN_RD]==FALSE)
	{
		glBegin(GL_TRIANGLE_FAN);
			//Center
			glColor3f(1.0,0,0);
			SetTexCoord(tcx,tcy);
			glVertex3f(cx,cy,m_pTerrain->GetValue(cx,cy));

			glColor3f(1.0,1.0,1.0);
			//down
			if(m_neighbor[NV_DOWN]==VS_ACTIVE)
			{
				SetTexCoord(tcx,tcy - tcd);
				glVertex3f(cx,cy - d ,m_pTerrain->GetValue(cx,cy - d));
#ifdef _COUNT_TRIAN_COUNT
			m_trian_count ++;
#endif
			}
             
			//right down
			SetTexCoord(tcx + tcd,tcy - tcd );
			glVertex3f(cx + d,cy - d ,m_pTerrain->GetValue(cx + d,cy - d));

			//right
			if(m_neighbor[NV_RIGHT]==VS_ACTIVE)
			{
				SetTexCoord(tcx + tcd,tcy);
				glVertex3f(cx + d,cy ,m_pTerrain->GetValue(cx + d,cy));
			}
			else
			{
				SetTexCoord(tcx + tcd,tcy + tcd);
				glVertex3f(cx + d,cy + d,m_pTerrain->GetValue(cx + d,cy + d));
			}
#ifdef _COUNT_TRIAN_COUNT
			m_trian_count ++;
#endif          
		glEnd();
	}


	//Right up
	if(m_subnode[SN_RU]==FALSE)
	{
		glBegin(GL_TRIANGLE_FAN);
			//Center
			glColor3f(1.0,0,0);
			SetTexCoord(tcx,tcy);
			glVertex3f(cx,cy,m_pTerrain->GetValue(cx,cy));

			glColor3f(1.0,1.0,1.0);
			//rihgt
			if(m_neighbor[NV_RIGHT]==VS_ACTIVE)
			{
				SetTexCoord(tcx + tcd,tcy);
				glVertex3f(cx + d,cy ,m_pTerrain->GetValue(cx + d,cy));
#ifdef _COUNT_TRIAN_COUNT
			m_trian_count ++;
#endif			
			}
             
			//right up
			SetTexCoord(tcx + tcd,tcy + tcd );
			glVertex3f(cx + d,cy + d ,m_pTerrain->GetValue(cx + d,cy + d));

			//Up
			if(m_neighbor[NV_UP]==VS_ACTIVE)
			{
				SetTexCoord(tcx ,tcy + tcd);
				glVertex3f(cx ,cy + d ,m_pTerrain->GetValue(cx,cy + d));
			}
			else
			{
				SetTexCoord(tcx - tcd,tcy + tcd);
				glVertex3f(cx - d,cy + d,m_pTerrain->GetValue(cx - d,cy + d));
			}
#ifdef _COUNT_TRIAN_COUNT
			m_trian_count ++;
#endif          
		glEnd();
       
	}
}

void CLOD::SetLodMethod(int method)
{
	m_lodmethod = method;
}

int CLOD::GetLodMethod()
{
	return m_lodmethod;
}

void CLOD::CullNode(int cx, int cy, int d)
{
	int sx = cx - d;
	int ex = cx + d;
	int sy = cy - d;
	int ey = cy + d;


	m_active.SetValue(cx,cy, VS_ACTIVE);


	d /= 2;
	while(d>1)
	{

		for( int x = sx + d; x < ex ;x += 2*d)
		{
			m_active.GetValue(x,d)= VS_ACTIVE;
			m_active.GetValue(x,ey - d) = VS_ACTIVE;
		}

		for( int y = sy + d; y < ey ;y += 2*d) 
		{
			m_active.GetValue(d,y)=VS_ACTIVE;
			m_active.GetValue(ex - d,y)=VS_ACTIVE;
		}

		d/=2;
	}
}


void CLOD::EnableGridRender(BOOL eGr)
{
	m_is_Grid = eGr;
}

void CLOD::CheckNeighbor(int cx, int cy, int d)
{
	int nx,ny;
	int d2= 2*d;

	m_neighbor[NV_LEFT]  = VS_ACTIVE;
	m_neighbor[NV_UP]    = VS_ACTIVE;
	m_neighbor[NV_DOWN]  = VS_ACTIVE;
	m_neighbor[NV_RIGHT] = VS_ACTIVE;
	//Test the four neighbor if they are all enabled/stoped
	nx = cx;ny = cy - d2;
	if(ny >0)
	{
		if(m_active.GetValue(nx,ny) == VS_DISABLE)
			m_neighbor[NV_DOWN] = VS_DISABLE;
	}

	nx = cx;ny = cy + d2;
	if(ny<m_h)
	{
		if(m_active.GetValue(nx,ny) == VS_DISABLE)
			m_neighbor[NV_UP] = VS_DISABLE;
	}

	nx = cx + d2;ny = cy;
	if(nx <m_w )
	{
		if(m_active.GetValue(nx,ny) == VS_DISABLE)
			m_neighbor[NV_RIGHT] = VS_DISABLE;
	}

	nx = cx - d2;ny = cy;
	if(nx > 0 )
	{
		if(m_active.GetValue(nx,ny) == VS_DISABLE)
			m_neighbor[NV_LEFT] = VS_DISABLE;
	}
}