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




  模块名称: Terrain Map
  功能描述：提供对地形数据的读取，
            预计支持自定义的地图格式。

  版本：Ver l.0
  作者：潘李亮 2003 - 3

**********************************************************/
#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#define TE_INVALIDATE_SIZE 2
#define TE_FILE_ERROR      1
#define TE_OK              0
#include "LOD.h"
#include "Camera.h"
#include "PageArray.h"

#include "map.h"

class CTerrain
{
	friend class CLOD;

	float         GetValue(int x,int y){return m_data_array.GetValue(x,y);}
    unsigned char*CaculateLightMap(float light_x,float light_y,float light_h,float scale);

public:
	CTerrain();
	~CTerrain()
	{
		FreeRPMap(m_map);
	//	m_data = NULL;
		if(m_pLodControl) 
			delete m_pLodControl;
	}

public:
	void          SetCamera(CCamera* pcm);
	CLOD*         GetLODControl();
	CLOD*         CreateLODCntrl();
	int           SetTexNum(int num =-1){if(num ==-1) return m_texNum; m_texNum = num;}
	int*          GetTexArray(){return m_texID;}
//	unsigned char*GetHeigtInfo(){return m_data;}
	void          SetStrcetch(float s){m_stretch = s;}
	int           GetX(){return m_w;}
	int           GetY(){return m_h;}
	float         GetScale(){return m_scale;}
	void          SetScale(float s){ m_scale = s;}
	float         operator()(int x,int y){return m_data_array.GetValue(x,y);}
	unsigned char*        operator[](int y){};//return &m_data[y*m_w];} 


public:
	GLuint BuilMainTexture(char *filename, float light_x,float light_y,float light_z,float scale);
	int GetRawValue(int x,int y);
	void Smooth(int level);
	void Strecth(float s);

	int		  LoadMap(char* file_name,float scaleW,float scaleH,float startH);
	int		  LaadFromRAW(char* file_name,float scaleW,float scaleH,float startH);
	int       SetTexture(GLuint tex[],int texNumb);
	int       Render();
	float     GetHeight(float x,float y);

	
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
private:
	int           m_w,m_h;
	int           m_texNum;
	int           m_texID[16];
	RPMAP         m_map;
	CCharArray    m_data_array;
	float         m_scale;
	float         m_stretch;
    CLOD*         m_pLodControl;
	CAMERA_INFO*  m_camera_info;
};


#endif
