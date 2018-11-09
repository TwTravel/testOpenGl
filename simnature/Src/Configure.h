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