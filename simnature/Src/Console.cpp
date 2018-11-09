/***************************************************************
   ģ������:
       Console.cpp
   Ŀ��: 
      ����һ������̨��

   ����:
        ������ HeartBlue
		       XpertSoft HeartBlue 2002/9/29
			   Allright reserved


    ����һ������̨�����ǿ��԰ѳ����״̬��Ϣ���������̨��
	�����������̨������־�ļ������á�
                    ������ 2003 - 4

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
����̨�Ĵ��ڴ�������
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
����һ������̨��Ҳ���Ǵ���һ������
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
                            "SimNature ����̨",
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
��ӡһ����Ϣ������̨�����ϻ���ʾ����
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

	//�õ�����̨�ܴ�ӡ�ĺ������Լ��о�
    dh =  tm.tmHeight + 5;
	max_line = (rct.bottom - rct.top)/dh; 

	//���ַ������뵽����̨��Ϣ���С�
	string s = text;
	m_buffer.push_back(s);

	//�趨��ʼ���к�
	//����ȫ����ӡ
	if(max_line < m_buffer.size())
	{

		//���������Ȱ���ǰ����Ϣ�߳�
       sl = m_buffer.size()-max_line;
	   SetTextColor(hdc,RGB(0,0,0));
	   SetBkColor(hdc,RGB(0,0,0));
	   //�ñ���дһ��
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
��ʽ�����
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
�޸Ļ��������һ�е���Ϣ��
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
���浽��־�ļ�����HTML��ʽ���ļ�
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
	f<<"<TITLE>SimNature ����̨ ��־�ļ� </TITLE>"<<endl;
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
�ŵ�����������ǲ���������ʾ����
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
