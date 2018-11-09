
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
	int			m_xWindow;       //���ڵĿ��
	int			m_yWindow;       //���ڵĸ߶�
	TCHAR*		m_pszClassName;   //���ڵ�����
	TCHAR*		m_pszAppTitle;    //���ڵı���
	HWND		m_hMainWnd;          //���ڵľ��
	HINSTANCE	m_hApp;          //�����ʵ�����
	HACCEL      m_hAccel;        //��ݼ�
	GAME_STAT   m_gamestat;     //��Ϸ�Ƿ񼤻�
	int			m_xLine;         //ˮƽ�ֱ���
	int			m_yLine;         //��ֱ�ֱ���
	int			m_colorBit;      //ɫ��
	BOOL        m_bIsFullScreen;        //�Ƿ�Ϊ��ռģʽ
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