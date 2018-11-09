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
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_
#include <windows.h>
#include <gl\gl.h>
#include "ViewFrustum.h"

//Billboard draw callback function
typedef void (*BILLBOARDDRAW)(int);
class C2DBillBoardMgr;
class C2DBillBoard
{
   friend class C2DBillBoardMgr;	
public:
	float         m_x,m_y,m_z;
	float         m_width,m_height;
	BILLBOARDDRAW m_pfnDraw;

protected:
	C2DBillBoard(BILLBOARDDRAW pfun = NULL);	
public:	

	//绘制一个BillBoard;
	void Draw(float theta,MVECTOR vUp);
	void DefDraw();
	void SetUp(MVECTOR v);
	void SetDraw(BILLBOARDDRAW pfun);
	//void SetTexture(GLuint texID);
};
#endif