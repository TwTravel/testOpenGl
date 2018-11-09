/**********************************************************
  SimNature Ver1.0.0
  Copyright : Panliliang (Stanly Lee)
  2002 - 2003
  Alright reserved!
  All Noncommeric use is Free!

  ��������Ȩ���� 
             2003-2003
  ����������з���ҵ��ʹ��Ϊ��Ѵ��롣

  E- Mial: Panliliang@etang.com
		   Xheartblue@etang.com 




   ģ������:
       BillBoardMgr.cpp
   Ŀ��: 
      �ṩ��BillBoardϵͳ��������֧��;

  �汾��1.0 2003/3/14
  ���ߣ�������

  �汾��1.2 2003/4/29 �޸�
  ���ߣ�������
		���Ӷ�Container�����֧�֣���һ����������֧�ֶ�����ͼ��
		BillBoard;
	    ͳһ�ڴ�ռ�Ĺ���

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

	//����һ���յ�BillBoard
	C2DBillBoard* CreateBillBoard(BILLBOARDDRAW pfun = NULL)
	{
		return new C2DBillBoard(pfun);
	}
	//ָ���ü���
	void SetCuller(CViewFrustum* culler)
	{
		m_Culler=culler;
	}
	//�趨���е�BillBoard��ԭʼ����
    void     SetBillBoardDir(MVECTOR m)
	{
		m_vDir = m;
		Math.Normalize(m_vDir);
	}
	//����BillBoard�����Ϸ���
    void     SetBillBoardUp(MVECTOR m)
	{
		m_vUp = m;
		Math.Normalize(m_vUp);
	}


};

#endif 