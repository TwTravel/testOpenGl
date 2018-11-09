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
#ifndef   _CONFIGURE_H_ 
#define   _CONFIGURE_H_

#include <windows.h>
#include <set>
#include <vector>
#include <fstream>
using namespace std;

class CConfigure  
{
	
	struct CCfgItem
	{
		char _name[64];
		char* _data;		
	};
	struct CCfgSection
	{
		char      _name[128];
		vector<CCfgItem> _items;
	};
	
public:
	float GetFloat(char *sec, char *item);
	int GetInteger(char* sec,char* item);
	char* GetString(char* sec,char* item);
	BOOL GetBool(char* sec,char* item);
	void Save();
	void FreeData();
	void Trace(char* file);
	BOOL InsertItem(char * section,char* itemName,char* data);
	char* IsSectionName(char *buf);
	BOOL Load(char* filename);
	CConfigure();
	virtual ~CConfigure();
		
private:
		vector<CCfgSection> m_sections;
		char                m_cfgfile[256];
};
extern CConfigure Configure;
#endif 