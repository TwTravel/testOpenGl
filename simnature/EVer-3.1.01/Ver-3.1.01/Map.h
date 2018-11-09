#ifndef _DLL_MAP_H_
#define _DLL_MAP_H_
/*
 *
 * RenderPro 地图的操作函数。
 * 注意：一个Bitmap格式的地图必须是8位深度的。
 *
**/
typedef struct Map_info
{
	int   _width;
	int   _height;
	float _scale;
}MAPINFO,*LPMAPINFO;

typedef struct Light_Map
{
	MAPINFO        _info;
	unsigned char* _data;
}RPMAP,*LPRPMAP,LIGHTMAP,*LPLIGHTMAP,MAPTEXTURE,*LPMAPTEXTURE;

#ifdef _DEBUG
#pragma comment(lib,"mapd.lib")
#else
#pragma comment(lib,"map.lib")
#endif

extern "C" RPMAP    LoadRPMapFromBitmap(char* mapname);
extern "C" RPMAP    LoadRPMap(char* mapname);
extern "C" void     SaveRPMap(RPMAP map,char* mapname);
extern "C" void     FreeRPMap(RPMAP map);
extern "C" RPMAP    CreateRPMap(int w,int h,float scale);
extern "C" LIGHTMAP CreateLightMap(RPMAP map,float light_x,float light_y,float light_h,int isLight);
extern "C" void     SaveLightMap(LIGHTMAP map,char* mapname);
#endif