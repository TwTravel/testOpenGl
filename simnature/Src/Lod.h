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

#ifndef _LOD_H_
#define _LOD_H_

#define _COUNT_TRIAN_COUNT


#include "ViewFrustum.h"

typedef unsigned char VERTEX_STATE;
#define VS_DISABLE      0x00
#define VS_ACTIVE       0x01
#define VS_CULLED       0x02
#define VS_STOPED		0x04
#define VS_UNREACH		0x08


#define NV_LEFT  0
#define NV_RIGHT 1
#define NV_UP    2
#define NV_DOWN  3 

#define SN_LU  0
#define SN_LD  1
#define SN_RD  2
#define SN_RU  3

//Max LoD level = 20 mean's the that the max map
//size can reach 2^20 by 2^20= 1M  
#define MAX_LOD_LEVEL 20

#include <windows.h>
#include <math.h>
#include "camera.h"
#include "PageArray.h"
class CTerrain;
class CViewFrustum;
#include <vector>
using namespace std;

typedef void (*PFNSETTEXCOORD)(float u,float v);
class CLOD
{
	typedef struct _NODE
	{
		int _x,_y;
	}QNODE;


public :
	void EnableGridRender(BOOL eGr);
	void CullNode(int cx,int cy,int d);
	void DrawNode(int cx,int cy,int d);
	void DrawPrim(int cx,int cy);
	BOOL NodeCanDivid(int cx,int cy,int d);
	void DividNode(int cx,int cy,int d);
	void DisableNode(int cx,int cy,int d);
	void ChangeDetailTex(GLuint tex);
	float GetObjectError();
	void SetObjectError(float erro);
	float VarMax(int cx,int cy,int level);
	void BuildVarMap();
	void IncreaseTexDetail(float deta);
	void SetTexDetail(float detail);
	void EnableDetailTex(BOOL b);
	void AfterRender();
	void PreRender();
	bool IsViewerInRect(int cx,int cy,int d);

	CLOD(CTerrain* t);
	~CLOD()
	{
		m_active.Destory();
		m_variant.Destory();
		//if(m_active)
		//	delete [] m_active;
		//if(m_variant)
		//	delete[] m_variant;
	}
	void          SetViewer();
	void          SetSpaceError(float error){m_space_error = error*error;}
	float         GetSpaceError(){return sqrt(m_space_error);};
	void          SetCuller(CViewFrustum* culler){m_cull = culler;}
    VERTEX_STATE  CanActive(int x,int y,int d);
	void          InitLODInfo();
    void          RenderLOD();
	BOOL          Render(int * texID);


private:
	int                  m_delta[MAX_LOD_LEVEL];
	int                  m_maxlevel;
	float                m_space_error;
	float                m_object_error;
	float                m_error;
	int                  m_mesh_size;
	float                m_view_x,m_view_y,m_view_z; 
	CTerrain*            m_pTerrain;
	CViewFrustum*        m_cull;


	CFloatArray          m_variant;
	CCharArray           m_active;



	float                m_tex_Detail;
	BOOL                 m_is_Detail_tex;
	BOOL                 m_is_Grid;//这种模式的渲染有利于观察LOD网格的组成
	int                  m_w,m_h;
	PFNSETTEXCOORD       SetTexCoord;

	GLuint               m_dtex;
	int                  m_lodmethod;
	int                  m_neighbor[6];
	int                  m_subnode[6];
//	GLdouble			 m_mv[16];
//	GLdouble			 m_mp[16];
//	GLint                m_vp[4];

#ifdef _COUNT_TRIAN_COUNT
	private:
		long          m_trian_count;
	public:
		int  GetLodMethod();
		void SetLodMethod(int method);
		void DrawHalfNode(int cx,int cy,int d);
		void RenderLOD2();
		void CheckNeighbor(int cx,int cy,int d);
		long GetTrianglesCount(){return m_trian_count;}
#endif
};

#endif