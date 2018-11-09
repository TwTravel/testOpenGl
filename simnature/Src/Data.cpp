/**********************************************
   模块名称:
       Data.cpp
   目的: 
      数据文件。

   作者:
        潘李亮 2003 - 4
  
	这个文件里保存着所有的公用数据的信息。
也就是说，所有的全局数据基本都保存在这里
**********************************************/

#include <windows.h>
#include <GL\gl.h>

#include "Configure.h"
#include "OpenGLCom.h"
extern CConfigure MapCfg;
#define EYE_ANGLE (10.)
float   g_culler_angle = 15;
float   g_streth = 1.0;
float   g_eye_h  = 2.2;
float   g_eye_angle= EYE_ANGLE;
float   g_step = 0.050;
float   g_space_error = 60;
float   g_object_error= 50;
float   g_flar_h = 200;
bool    g_cross  = true;
bool    g_iszoom  = false;
float   zoom_dist = 50;
float   zoom_angle      = 10;
float   g_view_dist     = 512;
int     MB=2000;
int     g_sb_h = 400;
BOOL    g_is_detail_tex = FALSE;

bool    is_bb_draw = false;
bool    g_isFog = false;
bool    stop  = false;

/****************************************************
从配置文件中读取配置信息
****************************************************/
void  ApplyConfigure()
{
	
	g_isFog          = Configure.GetBool("General","fog");
	is_bb_draw       = Configure.GetBool("General","billboard");
	g_eye_h          = Configure.GetFloat("General","eye_h"); 
	g_GL.m_eye_angle = Configure.GetFloat("General","eye_angle"); 
	g_step           = Configure.GetFloat("General","step"); 
	g_space_error    = Configure.GetInteger("LOD","space_error");
	g_view_dist      = Configure.GetFloat("LOD","viewdist");
	g_culler_angle   = Configure.GetFloat("LOD","angle");
	g_object_error   = Configure.GetFloat("LOD","object_error");

	g_sb_h           = MapCfg.GetFloat("SkyBox","height");
	g_streth         = MapCfg.GetFloat("Map","strength");
 	MB               = MapCfg.GetInteger("BillBoard","count");
    g_is_detail_tex  = MapCfg.GetBool("Map","is_detail_tex");
	zoom_angle       = g_GL.m_eye_angle/3.;

}