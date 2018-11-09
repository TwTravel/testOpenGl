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




  模块名称: Console
  功能描述：提供一个控制台，程序可以向控制台输出信息，
			本程序在启动之前，将所有的初始化信息输出到控制台

  版本：1.0
  作者：潘李亮 2003-4-24

**********************************************************/
#ifndef  _CONSOLE_H_
#define  _CONSOLE_H_

#include <windows.h>
#include <vector>
#include <fstream>
#include <locale>

using namespace std;

class CConsole  
{
public:
	void Log(char* format,...);
	void SaveToLogFile(char* file = NULL);
	void RePrintf(char* format,...);
	void Clear();
	void Printf(char* format,...);
	void Show();
	void Hide();
	void Close();
	void Print(char* text);
	BOOL Open();
	CConsole();
	virtual ~CConsole();

private:
	HWND          m_hWnd;
	vector<string> m_buffer;
};
extern CConsole Console;

#endif 