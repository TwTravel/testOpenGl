/**********************************************************
  SimNature Ver1.0.0
  Copyright : Panliliang (Stanly Lee)
  2002 - 2003
  Alright reserved!
  All Noncommeric use is Free!

  潘李亮版权所有 
             2003-2003
  本程序的所有非商业化使用为免费代码。

  E- Mial: Panliliang@etang.com
		   Xheartblue@etang.com 




  模块名称:
  功能描述：

  版本：
  作者：

**********************************************************/
#ifndef _DATA_H_
#define _DATA_H_

extern float g_culler_angle ;
extern float g_streth;
extern float g_eye_h;
extern float g_step;
extern float g_space_error;
extern float g_object_error;
extern int   MB;
extern int   g_sb_h;
extern float g_flar_h;
extern bool  g_cross;
extern DWORD g_render_time;
extern DWORD g_update_time;
extern bool  g_iszoom;
extern float zoom_dist;
extern float  zoom_angle;
extern float   g_view_dist;
extern bool is_bb_draw;
extern bool g_isFog;
extern bool stop;
extern BOOL  g_is_detail_tex;
void  ApplyConfigure();

#endif