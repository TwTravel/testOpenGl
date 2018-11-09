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




  ģ������: Console
  �����������ṩһ������̨��������������̨�����Ϣ��
			������������֮ǰ�������еĳ�ʼ����Ϣ���������̨

  �汾��1.0
  ���ߣ������� 2003-4-24

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