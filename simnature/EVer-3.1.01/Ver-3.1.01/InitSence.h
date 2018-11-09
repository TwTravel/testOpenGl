
#ifndef _INIT_SENCE_H_
#define _INIT_SENCE_H_
#include "gameapp.h"
#include "texturemgr.h"
#include "lensfare.h"
#include "Billboard.h"
#include "SkyBox.h"
#include "terrain.h"
#include "lod.h"
#include "viewfrustum.h"
#include "Configure.h"
#include "camera.h"
#include "Water.h"

extern "C" CGameApp g_app;
extern "C" CTextureMgr g_tex_mgr;
extern "C" CLensFlar flare;
extern "C" C2DBillBoardMgr g_bbm;
extern "C" CSkyBox g_sky;
extern "C" CTerrain  terrain;
extern "C" CLOD*      pLOD;
extern "C" CViewFrustum Culler;
extern "C" CCamera* g_pcm;
extern "C" CWaters  g_Water;

void InitFog();
void SetCameraH();
BOOL  InitSky();
BOOL InitTexture();
BOOL InitTerrain();
void InitCamera();
void InitBillBoard();
void InitFlare();
void DrawCircle(float r,float z);
void DrawBillBoad(int id);
void DrawLensMask();
void DrawCross();
void LoadMapCfg();
void LoadAppCfg();
void TerminateApp();
BOOL InitMultiTex();
BOOL InitWater();


extern CConfigure MapCfg;
#endif