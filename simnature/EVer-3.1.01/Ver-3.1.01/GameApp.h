
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
#include "windows.h"
#include "stdio.h"
#ifndef APPCOM_H
#define APPCOM_H

#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")      
#pragma comment(lib,"winspool.lib")
#pragma comment(lib,"comdlg32.lib")
#pragma comment(lib,"shell32.lib")
#pragma comment(lib,"advapi32.lib")
#pragma comment(lib,"ole32.lib")
#pragma comment(lib,"oleaut32.lib")
#pragma comment(lib,"uuid.lib")
#pragma comment(lib,"odbccp32.lib")
#pragma comment(lib,"odbc32.lib")
enum GAME_STAT
{
	GS_EXIT = -1,
	GS_NOREADY =0,
	GS_RUN,
	GS_STOP,
	GS_START,
	GS_PAUSE,
	GS_GAMEOVER

};

typedef int (*PUPDATEFRAME)(LONG);
//============Golable Var==================
class CGameApp
{
public:
	~CGameApp();
	BOOL SetDisplayMode(int w,int h,int cl_bits,int display_fre);
	CGameApp(TCHAR* strAppTitle){m_pszAppTitle = strAppTitle;m_gamestat = GS_NOREADY;}
	BOOL		CreateExcApp(WNDPROC pfnWndProc,int w =0,int h =0,int nCmdShow=SW_SHOW);
	BOOL		CreateNormalApp(WNDPROC pfnWndProc,int nCmdShow=SW_SHOW,int posx=0,int posy=0,int width=0,int height=0,ULONG  uStyle=WS_OVERLAPPEDWINDOW);
	BOOL		EnterFullScreen(BOOL reDraw);
	LONG		DelWindowStyle(LONG newstyle);
	LONG		SetWindowStyle(LONG newstyle);
	BOOL		ExitFullScreen(BOOL reDraw);
	int			MainLoop( PUPDATEFRAME pfUpdateFrame=NULL );
	void		SetForceFps(int fps);

private: 
	RECT m_WindPos;
	LONG m_OldStyle;
public:
	void SetFullScreenMode(BOOL IsFullScreen,BOOL Redraw);
	void ShowWindow(UINT mode);
	void Exit();
	void Stop();
	void Run();
	void SetGameStat(GAME_STAT stat);
	GAME_STAT GameState();
	int			m_xWindow;       //窗口的宽度
	int			m_yWindow;       //窗口的高度
	TCHAR*		m_pszClassName;   //窗口的类名
	TCHAR*		m_pszAppTitle;    //窗口的标题
	HWND		m_hMainWnd;          //窗口的句柄
	HINSTANCE	m_hApp;          //程序的实例句柄
	HACCEL      m_hAccel;        //快捷键
	GAME_STAT   m_gamestat;     //游戏是否激活
	int			m_xLine;         //水平分辨率
	int			m_yLine;         //垂直分辨率
	int			m_colorBit;      //色深
	BOOL        m_bIsFullScreen;        //是否为独占模式
	int         m_fps;		   //
	int			m_forceFPS;
	BOOL		m_bForceFPS;
    GAME_STAT   m_stat;
	
};

extern CGameApp g_app;
extern TCHAR szClassName[];
extern TCHAR szAppTitle[];
//==========Golable Function==================
extern int RunApp(LPSTR ,int);
//=====================================================
//Function Name: WinMain
//Description
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow);
#endif