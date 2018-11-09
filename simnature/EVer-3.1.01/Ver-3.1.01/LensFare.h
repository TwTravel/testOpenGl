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
  ����������

  �汾��
  ���ߣ�

**********************************************************/
#ifndef _LENS_FLARE_H_
#define _LENS_FLARE_H_

#include<vector>
using namespace std;

#include "Camera.h"

class CLensFlar
{
	typedef struct
	{
		float   pos;
		float   size;
		int     texID;
	}LFNODE,*PLFNODE;


public:
	BOOL IsBackOfViewer();
	void SetCamera(CCamera* pCamera);
	
    void Release();
	
	void SetLocation(float x,float y,float z);
	void AddSubNode(float size,float pos,int texID);
	void AddMainNode(float size,int texID);
	
	void Draw();

private:
	vector<LFNODE> m_MainNodes;
	vector<LFNODE> m_SubNodes;

	float m_loca_x,m_loca_y,m_loca_z;
	float m_screen_x,m_screen_y;
	float m_tag_x,m_tag_y;
    CAMERA_INFO* m_pCamera;

	
};
#endif