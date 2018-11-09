/***************************************************************
   模块名称:
       Console.cpp
   目的: 
      建立一个控制台。

   作者:
        潘李亮 HeartBlue
		       XpertSoft HeartBlue 2002/9/29
			   Allright reserved


    建立一个控制台，我们可以把程序的状态信息输出到控制台。
	而且这个控制台还有日志文件的作用。
                    潘李亮 2003 - 4

***************************************************************/

#include "Console.h"
#include "resource.h"
#include "GameApp.h"
#include <locale>
using namespace std;
CConsole::CConsole()
{

}

CConsole::~CConsole()
{

}
/****************************************
控制台的窗口处理函数。
****************************************/
LRESULT CALLBACK	ConsoleWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_CREATE:
	case WM_SIZE:
	case WM_MOVE:
	break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	default:
		return ::DefWindowProc(hWnd,msg,wParam,lParam); 
	}
   return 1;

}
/********************************************
创建一个控制台，也就是创建一个窗口
********************************************/
BOOL CConsole::Open()
{

     WNDCLASSEX  wndclass ;
     wndclass.cbSize        = sizeof (wndclass) ;
     wndclass.style         = CS_HREDRAW | CS_VREDRAW;
     wndclass.lpfnWndProc   = ConsoleWndProc;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = g_app.m_hApp;//hInstance ;
     wndclass.hIcon         = LoadIcon (g_app.m_hApp, MAKEINTRESOURCE(IDI_MAIN) ) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH) ;
     wndclass.lpszMenuName  = "SimNature Console";
     wndclass.lpszClassName = "SimNature Console";
     wndclass.hIconSm       =  LoadIcon (g_app.m_hApp, MAKEINTRESOURCE(IDI_MAIN) ) ;
	 
	 // Register the class
	 RegisterClassEx(&wndclass);
    // Create a window
	int sw = GetSystemMetrics(SM_CXSCREEN); // x size
    int sh = GetSystemMetrics(SM_CYSCREEN);
	int sx = (sw - 600)/2;
	int sy = 20;
    m_hWnd = CreateWindowEx(
	                        0,//WS_EX_TOPMOST,  // Extended style
                            "SimNature Console",
                            "SimNature 控制台",
                            WS_OVERLAPPEDWINDOW&~WS_MAXIMIZEBOX,       // Window style
                            sx,              // Horizontal origin  
                            sy,              // Vertical origin
                            600, // x size
                            sh-40, // y size
                            NULL,            // Handle of parent
                            NULL,            // Handle to menu
                            g_app.m_hApp,//hInstance,       // Application instance
                            NULL);           // Additional data

    if (!m_hWnd)
        return FALSE;
    ShowWindow(m_hWnd,SW_SHOW);//nCmdShow);
	return TRUE;
}
/*************************************************
打印一行信息到控制台，马上会显示出来
*************************************************/
void CConsole::Print(char *text)
{
	TEXTMETRIC tm;
	RECT        rct;
	int         dh;
	int         max_line;
	int         sl;

	HDC hdc = GetDC(m_hWnd);
    GetTextMetrics(hdc,&tm);
    GetClientRect(m_hWnd,&rct);

	//得到控制台能打印的函数，以及行距
    dh =  tm.tmHeight + 5;
	max_line = (rct.bottom - rct.top)/dh; 

	//把字符串加入到控制台信息池中。
	string s = text;
	m_buffer.push_back(s);

	//设定开始的行号
	//不能全部打印
	if(max_line < m_buffer.size())
	{

		//首先我们先把以前的信息檫除
       sl = m_buffer.size()-max_line;
	   SetTextColor(hdc,RGB(0,0,0));
	   SetBkColor(hdc,RGB(0,0,0));
	   //用背景写一边
	   for(int i=0;sl<m_buffer.size();sl++,i++)
	   {
		   ::TextOut(hdc,0,i*dh,m_buffer[sl-1].c_str(),m_buffer[sl-1].length());
	   }
	   sl = m_buffer.size()-max_line;
	}
    else
	   sl = 0;

	SetTextColor(hdc,RGB(255,255,0));
	SetBkColor(hdc,RGB(0,0,0));
	for(int i=0;sl<m_buffer.size();sl++,i++)
	{
		::TextOut(hdc,0,i*dh,m_buffer[sl].c_str(),m_buffer[sl].length());
	}
	ReleaseDC(m_hWnd,hdc);
}

void CConsole::Close()
{
	DestroyWindow(m_hWnd); 
}

void CConsole::Hide()
{
	ShowWindow(m_hWnd,SW_HIDE);
}

void CConsole::Show()
{
	ShowWindow(m_hWnd,SW_SHOW);
}
CConsole Console;

/*************************************************
格式化输出
*************************************************/
void CConsole::Printf(char *format, ...)
{

 	char buffer[1024]; 
    va_list args;
    va_start(args, format);
    _vsnprintf(buffer, 1024, format, args);
    va_end(args);
    buffer[1023] = '\0';
	Print(buffer);

}

void CConsole::Clear()
{
	m_buffer.clear();
}
/*************************************************
修改缓冲区最后一行的信息。
*************************************************/
void CConsole::RePrintf(char *format, ...)
{
 	char buffer[1024]; 
    va_list args;
    va_start(args, format);
    _vsnprintf(buffer, 1024, format, args);
    va_end(args);
    buffer[1023] = '\0';
	m_buffer.pop_back();

	Print(buffer);
	
}
/*******************************************
保存到日志文件，是HTML格式的文件
*******************************************/
void CConsole::SaveToLogFile(char *file)
{
	char outname[256];
	if(file==NULL)
	{
		strcpy(outname,"Console.log.html");
	}
	else
	{
		strcpy(outname,file);
		strcat(outname,".html");
	}
	ofstream f;
	f.open(outname);
	f<<"<HTML>"<<endl;
	f<<"<HEAD>"<<endl;
	f<<"<TITLE>SimNature 控制台 日志文件 </TITLE>"<<endl;
	for(int i =0;i<m_buffer.size();i++)
	{
		f<<m_buffer[i].c_str();
        f<<"<br>";
	}
	f<<"</HEAD>"<<endl;
	f<<"</HTML>"<<endl;
	f.close();
}
/**********************************************
放到缓冲区里。但是不会马上显示出来
**********************************************/
void CConsole::Log(char *format, ...)
{
 	char buffer[1024]; 
    va_list args;
    va_start(args, format);
    _vsnprintf(buffer, 1024, format, args);
    va_end(args);
    buffer[1023] = '\0';
	string s=buffer;
	m_buffer.push_back(s);
}
