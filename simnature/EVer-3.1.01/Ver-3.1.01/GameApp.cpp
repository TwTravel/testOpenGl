/**********************************************************
       模块名： 
	     GameApp.cpp
		为创建一个简单的 Win32应用提供支持
		     magix - heartblue All right reserved
			 2001 - 10 -1
			 2002 - 8 - 3 提供了对帧率的锁定功能

  2003 - 4 - 10
     版本 2.0 : 提供一个应用程序的C++类表示
	 作者：潘李亮

***********************************************************/
#include "windows.h"
#include "stdio.h"
#include "GameApp.h"
#include "resource.h"
//============================================

TCHAR szClassName[] = "GameWnd";
//============================================
//Function Name :  CreateExcApp 
//Description:     创建一个独占模式的应用
//============================================
BOOL CGameApp::CreateExcApp(WNDPROC pfnWndProc,int w,int h,int nCmdShow)
{

	if(w ==0)
		w =	GetSystemMetrics(SM_CXSCREEN); // x size
	if(h==0)
        h = GetSystemMetrics(SM_CYSCREEN); // y size
     WNDCLASSEX  wndclass ;
     wndclass.cbSize        = sizeof (wndclass) ;
     wndclass.style         = CS_HREDRAW | CS_VREDRAW;
     wndclass.lpfnWndProc   = pfnWndProc;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = m_hApp;//hInstance ;
     wndclass.hIcon         = LoadIcon (m_hApp, MAKEINTRESOURCE(IDI_MAIN) ) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject
                              (BLACK_BRUSH) ;
     wndclass.lpszMenuName  = szClassName;
     wndclass.lpszClassName = szClassName;
     wndclass.hIconSm       = LoadIcon (m_hApp, MAKEINTRESOURCE(IDI_MAIN) ) ;
	 
	 // Register the class
	 RegisterClassEx(&wndclass);
     m_bIsFullScreen=TRUE;
    // Create a window
     m_hMainWnd = CreateWindowEx(
	                        0,  // Extended style
                            szClassName,
                            m_pszAppTitle,
                            WS_POPUP,       // Window style
                            0,              // Horizontal origin  
                            0,              // Vertical origin
                            w,//GetSystemMetrics(SM_CXSCREEN), // x size
                            h,//GetSystemMetrics(SM_CYSCREEN), // y size
                            NULL,            // Handle of parent
                            NULL,            // Handle to menu
                            m_hApp,//hInstance,       // Application instance
                            NULL);           // Additional data

    if (!m_hMainWnd)
        return FALSE;
    m_xWindow=(int)GetSystemMetrics(SM_CXSCREEN)*2/3.;
	m_yWindow=(int)GetSystemMetrics(SM_CYSCREEN)*2/3.;
    ::ShowWindow(m_hMainWnd,SW_HIDE);//nCmdShow);
	return TRUE;
}

//================================================================
//  Function Name:    CreateNormalApp  
//  Description:       创建一个普通模式的应用
//================================================================
BOOL CGameApp::CreateNormalApp(WNDPROC pfnWndProc,int nCmdShow,int posx,int posy,int width,int height,ULONG  uStyle)
{
     WNDCLASSEX  wndclass ;
     wndclass.cbSize        = sizeof (wndclass) ;
     wndclass.style         = CS_HREDRAW | CS_VREDRAW;
     wndclass.lpfnWndProc   = pfnWndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = m_hApp ;
     wndclass.hIcon         = LoadIcon (m_hApp, MAKEINTRESOURCE(IDI_MAIN) ) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject
                              (BLACK_BRUSH) ;
     wndclass.lpszMenuName  = szClassName;
     wndclass.lpszClassName = szClassName;
     wndclass.hIconSm       = LoadIcon (m_hApp, MAKEINTRESOURCE(IDI_MAIN) ) ;
	 
	 // Register the class
	 RegisterClassEx(&wndclass);
     m_bIsFullScreen=FALSE;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++


//================Create Window=================
  if(height==0)//Default size;            
     m_hMainWnd = CreateWindowEx(0,        // Extended style
                         szClassName,
                         m_pszAppTitle,
                         WS_OVERLAPPEDWINDOW,
				         CW_USEDEFAULT,
				         CW_USEDEFAULT,
				         CW_USEDEFAULT,
				         CW_USEDEFAULT,
				         NULL,            // Handle of parent
                         NULL,            // Handle to menu
                         m_hApp,       // Application instance
                         NULL);  // Additional data
  else
	  m_hMainWnd = CreateWindowEx(0,        // Extended style
                         szClassName,
                         m_pszAppTitle,
                         uStyle,
				         posx,
				         posy,
				         width,
				         height,
				         NULL,            // Handle of parent
                         NULL,            // Handle to menu
                         m_hApp,       // Application instance
                         NULL);  // Additional data
//=============================================================
   
    if (!m_hMainWnd)
        return FALSE;
	RECT rct;
	GetWindowRect(m_hMainWnd,&rct); 
	m_xWindow=rct.right-rct.left;
	m_yWindow=rct.bottom-rct.top;
    ::ShowWindow(m_hMainWnd,SW_HIDE);//nCmdShow);
	return TRUE;
}

//================================
//  Function Name:  SetFullScreen
//  Description  :  
//================================
BOOL CGameApp::EnterFullScreen(BOOL reDraw)
{

	m_bIsFullScreen = TRUE;
//Set A TopMost Window;
	LONG style=GetWindowLong(m_hMainWnd,GWL_EXSTYLE);
	style&=~WS_EX_TOPMOST;
	SetWindowLong(m_hMainWnd,GWL_EXSTYLE,style);

	RECT WndRect,ClientRect;

	GetWindowRect(m_hMainWnd,&WndRect);
	GetClientRect(m_hMainWnd,&ClientRect);

	m_OldStyle = GetWindowLong(m_hMainWnd,GWL_STYLE);

	SetWindowLong(m_hMainWnd,GWL_STYLE,WS_POPUP| WS_VISIBLE);

	//保存好当前的位置。
	GetWindowRect(m_hMainWnd,&m_WindPos);

	int sy = (WndRect.bottom - WndRect.top) - (ClientRect.bottom - ClientRect.top);  

	m_xWindow = GetSystemMetrics(SM_CXSCREEN);
	m_yWindow = GetSystemMetrics(SM_CYSCREEN);

	MoveWindow(m_hMainWnd,0,0,m_xWindow,m_yWindow,reDraw);
	
	return TRUE;
}

//===================================================
//  Function Name : Del/SetWindowStyle
//  Description   : Delete or Set A window Style
//===================================================
LONG  CGameApp::DelWindowStyle(LONG newstyle)
{
	LONG style=::GetWindowLong(m_hMainWnd,GWL_STYLE);
	style&=!newstyle;
	return SetWindowLong(m_hMainWnd,GWL_STYLE,style);
}
//--------------------------------------------------
LONG  CGameApp::SetWindowStyle(LONG newstyle)
{
	LONG style=GetWindowLong(m_hMainWnd,GWL_STYLE);
	style|=newstyle;
	return SetWindowLong(m_hMainWnd,GWL_STYLE,style);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++


//===============================================================
// Function Name:  ExitFullScreen
// Descrition:     
//================================================================
BOOL CGameApp::ExitFullScreen(BOOL reDraw)
{
	m_bIsFullScreen = FALSE;

	LONG style=GetWindowLong(m_hMainWnd,GWL_EXSTYLE);
	style&=~WS_EX_TOPMOST;
	SetWindowLong(m_hMainWnd,GWL_EXSTYLE,style);
	SetWindowLong(m_hMainWnd,GWL_STYLE,m_OldStyle);
	m_xWindow = m_WindPos.right - m_WindPos.left;
	m_yWindow = m_WindPos.bottom - m_WindPos.top; 

	MoveWindow(m_hMainWnd,m_WindPos.left,m_WindPos.top,m_xWindow,m_yWindow,reDraw);
	GetWindowRect(m_hMainWnd,&m_WindPos);
	m_xWindow = m_WindPos.right - m_WindPos.left;
	m_yWindow = m_WindPos.bottom - m_WindPos.top; 
	return TRUE;
}
//=====================================================
//Function Name: WinMain
//Description
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	       g_app.m_hApp=hInstance;
		   g_app.m_fps=0;
		   g_app.m_bForceFPS = FALSE;
           return RunApp(lpCmdLine,nCmdShow);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++

//================================================================
// Function name: MainLoop
// Description  : the Main message loop
//===============================================================
int CGameApp::MainLoop( PUPDATEFRAME pfUpdateFrame)
{
	MSG msg;
	DWORD  thisTick;
	DWORD  lastTick;
	int count=0;
	lastTick=::GetTickCount();
	while( 1 )
	{  
		 if(m_gamestat == GS_EXIT)
		 {
			 PostQuitMessage(0);
			 return -1;
		 }
		 if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
          {
              if( !GetMessage( &msg, NULL, 0, 0 ) )//Remove The message From the Message Queue
              {
                  return msg.wParam;
              }
              if ( !TranslateAccelerator( m_hMainWnd, m_hAccel, &msg ) ) 
			  {
                  TranslateMessage(&msg);
                  DispatchMessage(&msg);
              }
          }
          else if ( m_gamestat > 0 )
          {
			  thisTick=GetTickCount();
			  if(m_bForceFPS)//强制帧率的帧率计算
			  {
				  while((thisTick-lastTick)<m_forceFPS)
					  thisTick=GetTickCount();
				  int a=(int)1000/(thisTick-lastTick);
				  m_fps=a;//1000/(thisTick-lastTick);
				  lastTick=thisTick;
			  }//非强制帧率的帧率计算
			  else
			  {
				  if( (thisTick-lastTick)>1000)
				  {
					  m_fps=count*1000/(thisTick-lastTick);
					  count=0;
					  lastTick=thisTick;
				  }
				  else
					  count++;
			  }
			 if(pfUpdateFrame)
                  (*pfUpdateFrame)(m_fps);
          }
          else
          {   
              WaitMessage();
          }
      }
	return msg.wParam;
}

void CGameApp::SetForceFps(int fps)
{
	if(fps==0)
	{
	  m_bForceFPS=0;
	  return ;
	}
	m_forceFPS=1000/fps;
	m_bForceFPS=1;
	return;
}

BOOL CGameApp::SetDisplayMode(int w, int h, int cl_bits,int display_fre)
{
	DEVMODE dmSettings = {0};	
	if(!EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&dmSettings))
	{
		MessageBox(NULL, "Could Not Enum Display Settings", "Error", MB_OK);
		return FALSE;
	}

	dmSettings.dmPelsWidth	= w;	
	dmSettings.dmPelsHeight	= h;
	dmSettings.dmBitsPerPel = cl_bits;
	dmSettings.dmDisplayFrequency = display_fre;
	dmSettings.dmFields     = DM_DISPLAYFREQUENCY| DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	// This function actually changes the screen to full screen
	// CDS_FULLSCREEN Gets Rid Of Start Bar.
	// We always want to get a result from this function to check if we failed
	int result = ChangeDisplaySettings(&dmSettings,CDS_FULLSCREEN);	

	// Check if we didn't recieved a good return message From the function
	if(result != DISP_CHANGE_SUCCESSFUL)
	{
		// Display the error message and quit the program
		MessageBox(NULL, "Display Mode Not Compatible", "Error", MB_OK);
		return FALSE;
	}
	return TRUE;
}

CGameApp::~CGameApp()
{
	ChangeDisplaySettings(NULL,0);
}

GAME_STAT CGameApp::GameState()
{
	return m_gamestat;
}

void CGameApp::SetGameStat(GAME_STAT stat)
{
	m_gamestat = stat;
}

void CGameApp::Run()
{
	m_gamestat = GS_RUN;
}

void CGameApp::Stop()
{
	m_gamestat = GS_STOP;
}

void CGameApp::Exit()
{
	m_gamestat = GS_EXIT;
}

void CGameApp::ShowWindow(UINT mode)
{
	::ShowWindow(m_hMainWnd,mode);
	UpdateWindow(m_hMainWnd);
}

void CGameApp::SetFullScreenMode(BOOL IsFullScreen, BOOL Redraw)
{
	if(IsFullScreen == FALSE && this->m_bIsFullScreen == TRUE)
	{
		ExitFullScreen(Redraw);
		return;
	}
	if(IsFullScreen == TRUE && this->m_bIsFullScreen == FALSE)
	{
		EnterFullScreen(Redraw);
		return ;
	}
	return ;
}
