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
       BillBoardMgr.cpp
   目的: 
      提供对BillBoard系统分类管理的支持;

  版本：1.0 2003/3/14
  作者：潘李亮

  版本：1.2 2003/4/29 修改
  作者：潘李亮
		增加对Container概念的支持，在一个管理器里支持多种贴图的
		BillBoard;
	    统一内存空间的管理

**********************************************************/
#ifndef _BILL_BOARD_MGR_
#define _BILL_BOARD_MGR_



#include <vector>
#include "ViewFrustum.h"
#include "BillBoard.h"

using namespace std;
class C2DBillBoard;
class CViewFrustum;
class C2DBillBoardMgr  
{

	struct bb_Container
	{
	    GLuint        _texID;
		vector<C2DBillBoard*> _BillBoards;
	};
private:
    CAMERA_INFO*   m_cinfo;

	
	MVECTOR       m_vDir;
	MVECTOR       m_vUp;
	CViewFrustum* m_Culler;
	vector<bb_Container> m_Container;
public:
	C2DBillBoardMgr();
	~C2DBillBoardMgr();
public:
	C2DBillBoard* CreateBillBoard(MVECTOR,int width,int height,GLuint texID);
	void Draw();
	void Add(C2DBillBoard* bb,int index);
	void SetCameraInfo(CCamera* pcm);
	int CreateContainer(GLuint texID);
public:
	int GetTypeCount();

	//创建一个空的BillBoard
	C2DBillBoard* CreateBillBoard(BILLBOARDDRAW pfun = NULL)
	{
		return new C2DBillBoard(pfun);
	}
	//指定裁剪器
	void SetCuller(CViewFrustum* culler)
	{
		m_Culler=culler;
	}
	//设定所有的BillBoard的原始方向
    void     SetBillBoardDir(MVECTOR m)
	{
		m_vDir = m;
		Math.Normalize(m_vDir);
	}
	//设置BillBoard的向上方向
    void     SetBillBoardUp(MVECTOR m)
	{
		m_vUp = m;
		Math.Normalize(m_vUp);
	}


};

#endif 