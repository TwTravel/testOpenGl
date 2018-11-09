/**********************************************************
   模块名称:
       InitSence.cpp
   目的: 
       这里的函数提供了场景的初始化;

   作者:
        潘李亮 2003 - 4 

    

**********************************************************/
#include "configure.h"
#include "initsence.h"
#include "viewfrustum.h"
#include "camera.h"
#include "viewercamera.h"
#include "Texturemgr.h"
#include "BillBoardmgr.h"
#include "LOD.h"
#include "Data.h"
#include "map.h"
#include "Terrain.h"
#include "image.h"
#include "Console.h"
#include "Texturemgr.h"
#include <gl\glext.h>

CConfigure MapCfg;

void TerminateApp()
{
		Console.Printf("程序将在 2 秒钟后自动关闭");
		Console.Printf("请检查Log文件");
		Console.SaveToLogFile();
		Sleep(2000);
		exit(0);
}
void InitFog()
{
	GLfloat color[]={Configure.GetFloat("Fog","r"),
		             Configure.GetFloat("Fog","g"),
					 Configure.GetFloat("Fog","b"),
					 Configure.GetFloat("Fog","a")};
	glFogi(GL_FOG_MODE,GL_EXP2);
	glFogfv(GL_FOG_COLOR,color);
	glFogf(GL_FOG_DENSITY,Configure.GetFloat("Fog","density"));
	glHint(GL_FOG_HINT,GL_NICEST);
	glFogf(GL_FOG_START,Configure.GetFloat("Fog","near"));
	glFogf(GL_FOG_END,Configure.GetFloat("Fog","far"));

	if(g_isFog)
		glEnable(GL_FOG);

}
BOOL InitSky()
{
	GLuint  leftTex;
	GLuint  rightTex;
	GLuint  topTex;
	GLuint  frontTex;
	GLuint  backTex;
	GLuint  bottomTex;

	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
//Load front Texture;
    frontTex = g_tex_mgr.LoadTexture(MapCfg.GetString("SkyBox","front"));
	if(frontTex == -1)
	{
		Console.Printf("纹理文件加载错误: %s",MapCfg.GetString("SkyBox","front"));
		goto exit_error;
	}
//Load back texture;
    backTex = g_tex_mgr.LoadTexture(MapCfg.GetString("SkyBox","back"));
	if(backTex == -1)
	{
		Console.Printf("纹理文件加载错误: %s",MapCfg.GetString("SkyBox","back"));
		goto exit_error;
	}
//Load top texture;
	topTex = g_tex_mgr.LoadTexture(MapCfg.GetString("SkyBox","top"));
	if(topTex == -1)
	{
		Console.Printf("纹理文件加载错误: %s",MapCfg.GetString("SkyBox","top"));
		goto exit_error;
	}
//Load left texture;
	leftTex = g_tex_mgr.LoadTexture(MapCfg.GetString("SkyBox","left"));
	if(leftTex == -1)
	{
		Console.Printf("纹理文件加载错误: %s",MapCfg.GetString("SkyBox","left"));
		goto exit_error;
	}
//Load right texture;
	rightTex = g_tex_mgr.LoadTexture(MapCfg.GetString("SkyBox","right"));
    if(rightTex == -1)
	{
		Console.Printf("纹理文件加载错误: %s",MapCfg.GetString("SkyBox","right"));
		goto exit_error;
	}

	//Load bottom texture;
	bottomTex = g_tex_mgr.LoadTexture(MapCfg.GetString("SkyBox","bottom"));
    if(bottomTex == -1)
	{
		Console.Printf("纹理文件加载错误: %s",MapCfg.GetString("SkyBox","bottom"));
		goto exit_error;
	}
	g_sky.InitSkyBox(terrain.GetX() ,terrain.GetY() ,g_sb_h,frontTex,backTex,leftTex,rightTex,topTex,bottomTex);
	g_sky.Adjust(MapCfg.GetInteger("SkyBox","start_x"),
		         MapCfg.GetInteger("SkyBox","start_y"),
				 MapCfg.GetInteger("SkyBox","start_h"));

	g_sky.SetCamera(g_pcm);
	return TRUE;

exit_error:
	Console.Printf("加载天空体纹理失败!");
	::TerminateApp();
	return FALSE;
}

BOOL  InitTerrain()
{
	GLuint  terrTex[16];
	Console.Printf("地图文件:%s",MapCfg.GetString("Map","mapfile"));
	Console.Printf("开始加载地图...Please Waiting ~_*");
	int rt = terrain.LoadMap(MapCfg.GetString("Map","mapfile") ,1.0,g_streth,0.);
	if(TE_OK != rt)
	{
		Console.Printf("地图加载失败: %s",MapCfg.GetString("Map","mapfile"));
		if(TE_INVALIDATE_SIZE == rt)
		{
			Console.Printf("地图尺寸不符合要求");
		}
		if(TE_FILE_ERROR == rt)
		{
			Console.Printf("地图文件错误");
		}
		TerminateApp();
		return FALSE;
	}

	Console.Printf("地图加载完毕，地图尺寸: %d X %d ",terrain.GetX(),terrain.GetY());
	

	float lx = MapCfg.GetFloat("LensFlar","pos_x");
	float ly = MapCfg.GetFloat("LensFlar","pos_y");
	float lz = MapCfg.GetFloat("LensFlar","pos_h");

	Console.Printf("地图纹理文件:%s",MapCfg.GetString("Map","maintex"));
	Console.Printf("开始加载地图纹理...Please waiting *_^");

	//加载纹理数据，计算地图的亮度图，并且把他们混合在一起

	if(MapCfg.GetBool("Map","lighttex") == false)
	{
		Console.Printf("计算亮度图，混合纹理...Please waiting *_^"); 
		terrTex[0] = terrain.BuilMainTexture(MapCfg.GetString("Map","maintex"),lx,ly,lz,g_streth);
		if( terrTex[0] == -1)
		{
			Console.Printf("地图纹理加载失败: %s",MapCfg.GetString("Map","maintex"));
			TerminateApp();
		}
		g_tex_mgr.AddTexture(terrTex[0]); 
	}
	else
	{
		terrTex[0] = g_tex_mgr.LoadTexture(MapCfg.GetString("Map","maintex"));
	}



	terrTex[1] =  g_tex_mgr.LoadTexture(MapCfg.GetString("Map","detail1"));
	if(terrTex[1] == -1)
	{
		Console.Printf("地图纹理加载失败: %s",MapCfg.GetString("Map","detail1"));
		TerminateApp();
	}


	terrTex[2] =  g_tex_mgr.LoadTexture(MapCfg.GetString("Map","detail2"));
	if(terrTex[2] == -1)
	{
		Console.Printf("地图纹理加载失败: %s",MapCfg.GetString("Map","detail2"));
		TerminateApp();
	}


	terrTex[3] =  g_tex_mgr.LoadTexture(MapCfg.GetString("Map","detail3"));
	if(terrTex[3] == -1)
	{
		Console.Printf("地图纹理加载失败: %s",MapCfg.GetString("Map","detail3"));
		TerminateApp();
	}

	terrTex[4] =  g_tex_mgr.LoadTexture(MapCfg.GetString("Map","detail4"));
	if(terrTex[4] == -1)
	{
		Console.Printf("地图纹理加载失败: %s",MapCfg.GetString("Map","detail4"));
		TerminateApp();
	}

	terrain.SetTexture(terrTex,5);
    Console.Printf("地图纹理加载完毕");


	Console.Printf("正在平滑地图数据( %d次)!...Please waiting *_^",MapCfg.GetInteger("Map","smoothpass"));
    terrain.Smooth(MapCfg.GetInteger("Map","smoothpass"));

	Console.Printf("创建LOD控制器.....");
	pLOD = terrain.CreateLODCntrl();
	
	Console.Printf("正在计算地形 Varaint数据...Please waiting *_^");
	pLOD->BuildVarMap();

	Console.Printf("地形Varaint 数据计算完毕");
	Console.Printf("Space---Error: %g",g_space_error);
	pLOD->SetObjectError(g_object_error); 
	Console.Printf("Object---Error: %g",g_object_error);
	pLOD->SetSpaceError(g_space_error);
	Console.Printf("给地形LOD控制器分配裁剪器");
	pLOD->SetCuller(&Culler);

	float texd =MapCfg.GetFloat("Map","texdetail");
	Console.Printf("地图细节纹理的细节值: %f",texd);
	pLOD->SetTexDetail(texd);
	Console.Printf("多遍纹理映射功能 Enabled!");
	pLOD->EnableDetailTex(g_is_detail_tex); 

	pLOD->ChangeDetailTex(1); 

	Console.Printf("视距 :%f",g_view_dist);
	Culler.SetViewerDist(g_view_dist); 
	Console.Printf("LOD 控制器参数设置完毕");
    
	return TRUE;
}
void InitCamera()
{
	int x = terrain.GetX()/2;//rand()%terrain.GetX();
	int y = terrain.GetX()/2;//rand()%terrain.GetY();
	float ch=terrain.GetHeight(x,y);

	g_pcm = new CViewerCamera;
	g_pcm->SetCameraPos(x, y    ,  ch + g_eye_h);
	g_pcm->SetUpDir(0,0,1);
	g_pcm->SetTarget( x, y+10 ,  ch + g_eye_h);
	g_pcm->SetViewerDir(0 , 1 , 0); 
	g_pcm->SetViewerUpDir( 0 , 0 , 1); 

	g_pcm->SetViewDist(g_view_dist);
	g_pcm->SetViewerAngle(g_GL.m_eye_angle);
	g_pcm->SetZoomAngle (g_GL.m_eye_angle/5);
	g_pcm->Zoomed(false);
	Culler.SetTerrain(&terrain);
}

void InitBillBoard()
{

	int ci;
	

	for(int loop = 0;;loop++)
	{
		GLubyte color[4] = {255,255,255,0};

	
		char buf[15];
		sprintf(buf,"tree%d",loop+1);
		char* filename = MapCfg.GetString("BillBoard",buf);
		if(filename == NULL)
			break;

		DIBTEXDATA dib = LoadTexData(filename);

		if(dib.pdata == NULL)
			break ;

		GetTexPixel(0,0,color,dib);

		FreeDibTexData(dib);



		GLuint treeTex = BuildTransparencyTexture(filename,color,0,4);
		if(treeTex == -1)
		{
			Console.Printf("树木纹理加载失败: %s", filename);
			TerminateApp();
		}
		else
		{
			g_tex_mgr.AddTexture(treeTex); 
			glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
			ci = g_bbm.CreateContainer(treeTex); 
		}

	}
/*
	GLubyte color2[4] = {255,0,255,0};
	treeTex = BuildTransparencyTexture(MapCfg.GetString("BillBoard","tree2"),color2,0,120);
	if(treeTex == -1)
	{
		Console.Printf("树木纹理加载失败: %s", MapCfg.GetString("BillBoard","tree2"));
		TerminateApp();
	}
    else
	{
		g_tex_mgr.AddTexture(treeTex); 
		glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
		ci = g_bbm.CreateContainer(treeTex); 
	}
*/
	int typecount = g_bbm.GetTypeCount();
	if(typecount ==0)
		return;

	CAMERA_INFO* cinfo= g_pcm->GetCameraInfo();

	MVECTOR mdir={0,-1,0,1};
	g_bbm.SetBillBoardDir(mdir);
    g_bbm.SetBillBoardUp(cinfo->vup);
	g_bbm.SetCameraInfo(g_pcm); 
    g_bbm.SetCuller(&Culler);
	int i;
	int  h = MapCfg.GetInteger("BillBoard","height");
	int  w = MapCfg.GetInteger("BillBoard","width");
	C2DBillBoard* bb;
	for(i=0;i<MB;i++)
	{
		bb = g_bbm.CreateBillBoard();
		float x,y;
		bb->m_height=(rand()%(h/2)+h/2)/3.0; 
		bb->m_width =(rand()%(w/2)+w/2)/3.0; 
		bb->m_x =x=rand()%(terrain.GetX()-5) +2 + (rand()%8)/ 7.0;
		bb->m_y =y=rand()%(terrain.GetY()-5) +2 + (rand()%8)/ 7.0;
		bb->m_z =terrain.GetHeight(x,y)- 0.1*bb->m_height ;
		g_bbm.Add(bb,rand()%typecount);
	}
}
void InitFlare()
{   
	GLuint texSecond;
	GLuint texCross;
	GLuint texRing;
	GLuint texMain;
	GLuint texMain2;
	GLuint texSecond2;

	texSecond  =  g_tex_mgr.LoadTexture("data\\flare\\second1.jpg");
	if(texSecond == -1)
	{
	  Console.Printf("纹理文件加载失败 :%s","data\\flare\\second1.jpg");
      goto error_exit;;
	}

	texCross   =  g_tex_mgr.LoadTexture("data\\flare\\cross.jpg");
	if(texCross == -1)
	{
	  Console.Printf("纹理文件加载失败 :%s","data\\flare\\cross.jpg");
      goto error_exit;;
	}

	texRing    =  g_tex_mgr.LoadTexture("data\\flare\\ring.jpg"); 
	if(texRing == -1)
	{
	  Console.Printf("纹理文件加载失败 :%s","data\\flare\\ring.jpg");
      goto error_exit;;
	}


	texMain    =  g_tex_mgr.LoadTexture("data\\flare\\main1.jpg"); 
	if(texMain == -1)
	{
	  Console.Printf("纹理文件加载失败 :%s","data\\flare\\main1.jpg");
      goto error_exit;;
	}


	texSecond  =  g_tex_mgr.LoadTexture("data\\flare\\second1.jpg");
	if(texSecond == -1)
	{
	  Console.Printf("纹理文件加载失败 :%s","data\\flare\\second1.jpg");
      goto error_exit;;
	}


	texMain2   =  g_tex_mgr.LoadTexture("data\\flare\\main2.jpg"); 
	if(texMain2 == -1)
	{
	  Console.Printf("纹理文件加载失败 :%s","data\\flare\\main2.jpg");
      goto error_exit;
	}


	texSecond2 =  g_tex_mgr.LoadTexture("data\\flare\\second2.jpg");
	if(texSecond2 == -1)
	{
	  Console.Printf("纹理文件加载失败 :%s","data\\flare\\second2.jpg");
      goto error_exit;;
	}



	flare.SetLocation( MapCfg.GetInteger("LensFlar","pos_x"),
				       MapCfg.GetInteger("LensFlar","pos_y"),
		               MapCfg.GetInteger("LensFlar","pos_h"));
	
	flare.SetCamera(g_pcm); 

	flare.AddMainNode(100,texMain);
	flare.AddMainNode(30,texCross);
	flare.AddMainNode(30,texRing);
	flare.AddMainNode(80,texMain2);

	flare.AddSubNode(180,2.0,texSecond);
	flare.AddSubNode(80,1.0,texSecond);
	flare.AddSubNode(60,1.3,texSecond2);
	flare.AddSubNode(6,1.1,texMain);
	flare.AddSubNode(15,1.0,texSecond);
	flare.AddSubNode(20,1.0,texSecond2);
	flare.AddSubNode(30,-0.4,texSecond);
	flare.AddSubNode(0,0.90,texMain);
	flare.AddSubNode(10,-0.20,texMain);
    return ;

error_exit:
	Console.Printf("加载眩光体纹理失败！");
    TerminateApp();
}


void DrawCircle(float r,float z)
{
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0,0,z);
	float x,y;
	for(int i =0;i<=360;i++)
	{
		x = r * Math.Cos(i);
		y = r * Math.Sin(i);
        glVertex3f(x,y,z);
	}
	glEnd();
}
void DrawBillBoad(int id)
{
	glBegin(GL_QUADS);
    glTexCoord2f(0,0);	
		glVertex3f(-2,0,0);
	glTexCoord2f(1,0); 
		glVertex3f(2,0,0);
	glTexCoord2f(1,1);
		glVertex3f(2,0,4);
	glTexCoord2f(0,1);  
		glVertex3f(-2,0,4);
	glEnd();
}


void DrawLensMask()
{
	
	  RECT rt;
	  GetClientRect(g_app.m_hMainWnd,&rt);
	  glClearStencil(0);
	  float a =float(rt.right -rt.left)/(rt.bottom  - rt.top);
      glEnable(GL_STENCIL_TEST);
	  glStencilFunc(GL_ALWAYS,1,1);
	  //glClear(GL_STENCIL_BUFFER_BIT);

	  //glStencilOp(GL_REPLACE,GL_REPLACE,GL_REPLACE);
	  glStencilOp(GL_INVERT,GL_INVERT,GL_INVERT);
	  	glMatrixMode(GL_PROJECTION);
		glPushMatrix();//=================
		glLoadIdentity();
		glOrtho(-100,100,-100 / a,100  / a,-500.0,500.0);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();////////////
		glLoadIdentity();
	    glColor3f(1.,1.,1.);
		DrawCircle(50,-20);
	   // glStencilOp(GL_INVERT,GL_INVERT,GL_INVERT);
		glBegin(GL_LINES);
		   glVertex3f(-50,0,-30);
		   glVertex3f(50 ,0,-30);
		   glVertex3f(0,-50,-30);
		   glVertex3f(0,50,-30);
		glEnd();

		glPopMatrix();////////////
	    glMatrixMode(GL_PROJECTION);
		glPopMatrix();//======================

	glDisable(GL_STENCIL_TEST);
}

void DrawCross()
{
	
	RECT rt;
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	GetClientRect(g_app.m_hMainWnd,&rt);
	float a =float(rt.right -rt.left)/(rt.bottom  - rt.top);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-100,100,-100 / a,100  / a,-500.0,500.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(0.,1.,1.);
	glDisable(GL_DEPTH_TEST);
	glLineWidth(1.0);
	glBegin(GL_LINES);
		glVertex3f(-5,0,-30);
		glVertex3f(5 ,0,-30);
		glVertex3f(0,-5,-30);
		glVertex3f(0,5,-30);
	glEnd();
	glLineWidth(1);
	glColor4f(1.0,0,0,1);
	glPointSize(5.0);
	glBegin(GL_POINTS);
	glVertex3f(0,0,-30);
	glEnd();
	glPointSize(1.0);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	
	
    glPopAttrib();
}

void LoadMapCfg()
{
	BOOL bLoad = MapCfg.Load(Configure.GetString("Map","map"));
	if(bLoad == FALSE)
	{
		//[Map];
		MapCfg.InsertItem("Map","size"      , "1025");
		MapCfg.InsertItem("Map","maintex"   , "data\\map\\textrue2048.jpg");
		MapCfg.InsertItem("Map","mapfile"   , "data\\map\\map1024.map");
		MapCfg.InsertItem("Map","strength"  , "0.113");
		MapCfg.InsertItem("Map","texdetail","19");
		MapCfg.InsertItem("Map","detail1","data\\map\\detail1.jpg");
		MapCfg.InsertItem("Map","detail2","data\\map\\detail2.jpg");
		MapCfg.InsertItem("Map","detail3","data\\map\\detail3.jpg");
		MapCfg.InsertItem("Map","detail4","data\\map\\detail4.jpg");
		MapCfg.InsertItem("Map","detail5","data\\map\\detail5.jpg");
		MapCfg.InsertItem("Map","detail6","data\\map\\detail6.jpg");
		MapCfg.InsertItem("Map","detail7","data\\map\\detail7.jpg");
		MapCfg.InsertItem("Map","detail8","data\\map\\detail8.jpg");
		MapCfg.InsertItem("Map","is_detail_tex","ON");
		MapCfg.InsertItem("Map","smoothpass","3");
		MapCfg.InsertItem("Map","lighttex","ON"); 
		
		//[SkyBox]
		MapCfg.InsertItem("SkyBox","style"  , "BOX");
		MapCfg.InsertItem("SkyBox","left"   , "data\\skybox\\left.jpg");
		MapCfg.InsertItem("SkyBox","right"  , "data\\skybox\\right.jpg");
		MapCfg.InsertItem("SkyBox","front"  , "data\\skybox\\front.jpg");
		MapCfg.InsertItem("SkyBox","back"   , "data\\skybox\\back.jpg");
		MapCfg.InsertItem("SkyBox","top"    , "data\\skybox\\top.jpg");
		MapCfg.InsertItem("SkyBox","bottom" , "data\\skybox\\bottom.jpg");
		MapCfg.InsertItem("SkyBox","height" , "320");
		MapCfg.InsertItem("SkyBox","start_h", "-100");
		MapCfg.InsertItem("SkyBox","start_x", "0");
		MapCfg.InsertItem("SkyBox","start_y", "0");
			
		//[LensFlar]
		MapCfg.InsertItem("LensFlar","pos_h","200");
		MapCfg.InsertItem("LensFlar","pos_x","0");
		MapCfg.InsertItem("LensFlar","pos_y","0");
			
		//	[BillBoard]
		MapCfg.InsertItem("BillBoard","count","1000");
		MapCfg.InsertItem("BillBoard","tree1","data\\texture\\tree1.jpg");
		MapCfg.InsertItem("BillBoard","tree2","data\\texture\\tree2.jpg");
		MapCfg.InsertItem("BillBoard","tree3","data\\texture\\tree3.jpg");
		MapCfg.InsertItem("BillBoard","tree4","data\\texture\\tree4.jpg");
		MapCfg.InsertItem("BillBoard","tree5","data\\texture\\tree5.jpg");
		MapCfg.InsertItem("BillBoard","height","50");
		MapCfg.InsertItem("BillBoard","width","40");
		MapCfg.Save();
	}
	
}

void LoadAppCfg()
{
	BOOL bLoad = Configure.Load("SimNature.ini");
	if(bLoad == FALSE)
	{
		//[General]
		Configure.InsertItem("General","screen_w","640");
		Configure.InsertItem("General","screen_h","480");
		Configure.InsertItem("General","color_depth","32");
		Configure.InsertItem("General","display_fre","85");
		Configure.InsertItem("General","fullscreen","ON");
		Configure.InsertItem("General","fog","ON");
		Configure.InsertItem("General","billboard","ON");
		Configure.InsertItem("General","eye_angle","25");
		Configure.InsertItem("General","eye_h","5.2");
		Configure.InsertItem("General","step","0.325");
		
		//[LOD]
		Configure.InsertItem("LOD","space_error","27");
		Configure.InsertItem("LOD","object_error","2.5");
		Configure.InsertItem("LOD","viewdist","600");
		Configure.InsertItem("LOD","angle","70");
		
		//[Map]
		Configure.InsertItem("Map","size","2049");
		Configure.InsertItem("Map","map","data\\map\\map2048.ini");
		
		//[Fog]
		Configure.InsertItem("Fog","r","0.8");
		Configure.InsertItem("Fog","g","0.8");
		Configure.InsertItem("Fog","b","0.9");
		Configure.InsertItem("Fog","a","1.0");
		Configure.InsertItem("Fog","density","0.00350");
		Configure.InsertItem("Fog","near","890");
		Configure.InsertItem("Fog","far","1000");

		//[Blur]
        Configure.InsertItem("Blur","x","0");
		Configure.InsertItem("Blur","y","0");
		Configure.InsertItem("Blur","value","0.7");
		Configure.InsertItem("Snip","mask","data\\texture\\sniper.jpg");
		Configure.Save();
	}
}