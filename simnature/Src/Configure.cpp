/***************************************************************
   模块名称:
       Configure.cpp
   目的: 
      提供对配置文件的读取

   作者:
        潘李亮 HeartBlue
		       XpertSoft HeartBlue 2002/9/29
			   Allright reserved


   配置文件分段，我们把配置文件读入到这个类中，然后按分段
   来保存，所有的信息为字符串，我们按照需要把字符串格式化
   成相应的信息
                    潘李亮 2003 - 4

***************************************************************/



#include <windows.h>
#include "Configure.h"
#include "GameApp.h"

CConfigure Configure;
CConfigure::CConfigure()
{

}

CConfigure::~CConfigure()
{

}
/*****************************************
         读入一个配置文件。
*****************************************/
BOOL CConfigure::Load(char *filename)
{
	ifstream in;
	char buf[256];
	char sec_name[64]="DEFAULT";
	char item_name[64];
	char item_data[256];

	char cur_section[64];

	strcpy(m_cfgfile,filename);

	in.open(filename);
	if(in.fail())
	{
		//TRACE(NULL,"配置文件不存在","配置文件错误！",MB_OK);
		return FALSE;
	}
	while(!in.eof())
	{
		in.getline(buf,128);
		//是一个段的标记，我们把当前的段设置为读入的这个段
		if(IsSectionName(buf))
		{
			strcpy(cur_section,buf+1);
			continue;
		}
		if(strlen(buf)==0)
			continue;
		//Not a  Section
		sscanf(buf,"%s =%s",item_name,item_data);
		InsertItem(cur_section,item_name,item_data); 
	}
	in.close();
	return TRUE;
}
/*****************************************
内部函数。
判断读入的一个字符串是不是一个段名。
段名的格式是: [SectionName]
*****************************************/
char* CConfigure::IsSectionName(char *buf)
{
	int len = strlen(buf);
	if(']' == buf[len-1] && buf[0] == '[')
	{
		buf[len-1]='\0';
		return buf + 1;
	}
	return NULL;
}
/*****************************************************
插入一个配置项目
section : 段名，即配置项所属的段的名字
itemname: 配置项的名
*****************************************************/
BOOL CConfigure::InsertItem(char *section, char *itemName, char *data)
{
	int sec_count = m_sections.size();

	for(int i = 0; i<sec_count ;i++)
	{
		//找到了一个已经有了的区
		if( strcmp(m_sections[i]._name,section) == 0)
		{
			for(int j = 0;j<m_sections[i]._items.size();j++)
			{
				//找到一个已经有了的一个配置项目，我们就修改它
				if(strcmp(itemName,m_sections[i]._items[j]._name)==0)
				{
					delete[] m_sections[i]._items[j]._data;
					m_sections[i]._items[j]._data = new char[strlen(data)+1];

					strcpy(m_sections[i]._items[j]._data,data);
					return TRUE;
				}
			}
			//没有找到
			CCfgItem newItem;
			newItem._data = new char[strlen(data)+1];

			strcpy(newItem._data,data);
			strcpy(newItem._name,itemName);
			
			m_sections[i]._items.push_back(newItem);
			return TRUE;
		}
	}
	//找不到指定的区。
	CCfgSection sec;
	strcpy(sec._name,section);
	
	CCfgItem newItem;
	newItem._data = new char[strlen(data)+1];
				
	strcpy(newItem._data,data);
	strcpy(newItem._name,itemName);
				
	sec._items.push_back(newItem);

	m_sections.push_back(sec);

	return TRUE;
}
/**************************************
用于调试目的，不需要关心
**************************************/
void CConfigure::Trace(char *file)
{
	ofstream out;
	out.open(file);
	for(int i=0;i<m_sections.size();i++)
	{
		out<<"Begin a new Section"<<endl;
		out<<endl;
		for(int j=0;j<m_sections[i]._items.size();j++)
		{
			out<<"    "<<m_sections[i]._items[j]._name<<":  "<<m_sections[i]._items[j]._data<<endl;   
		}
	}
	out.close();
}

/***********************************************************
释放所有的数据和数据的存储空间
***********************************************************/
void CConfigure::FreeData()
{
	for(int i=0;i<m_sections.size();i++)
		for(int j=0;j<m_sections[i]._items.size();j++)
		{
			delete [] m_sections[i]._items[j]._data;   
		}
	for(int k=0;k<m_sections.size();k++)
	{
		m_sections[k]._items.clear(); 
	}
	m_sections.clear();
}

/****************************************************
把配置文件写回到文件中，这样我们可以保存在程序运行时
对配置的修改
****************************************************/
void CConfigure::Save()
{
	//简单的对数据进行一次遍历操作
	ofstream out;
	out.open(m_cfgfile);
	for(int i=0;i<m_sections.size();i++)
	{
		out<<"["<<m_sections[i]._name<<"]"<<endl;
		for(int j=0;j<m_sections[i]._items.size();j++)
		{
			out<<m_sections[i]._items[j]._name<<" ="<<m_sections[i]._items[j]._data<<endl;   
		}
		out<<endl;
	}
	out.close();
}

/**********************************************
以下的函数为按照给出的段名，项的名字来查询信息。
并且把得到的信息格式化成所需要的信息
其中：GetString函数是关键
**********************************************/
BOOL CConfigure::GetBool(char *sec, char *item)
{

	char* buf = GetString(sec,item);
	if(buf == NULL)
		return FALSE;
	if(strcmp(buf,"ON"))
		return FALSE;
	return TRUE;
}

char* CConfigure::GetString(char *sec, char *item)
{ 
	int sec_count = m_sections.size();

	for(int i = 0; i<sec_count ;i++)
	{
		if( strcmp(m_sections[i]._name,sec) == 0)
			for(int j = 0;j<m_sections[i]._items.size();j++)
				if(strcmp(item,m_sections[i]._items[j]._name)==0)
					return m_sections[i]._items[j]._data; 

	}
	return NULL;
}

int CConfigure::GetInteger(char *sec, char *item)
{
	char* buf = GetString(sec,item);
	if(buf == NULL)
		return 0;
	int data;
	sscanf(buf,"%d",&data);
	return data;
}

float CConfigure::GetFloat(char *sec, char *item)
{
	char* buf = GetString(sec,item);
	if(buf == NULL)
		return 0;
	float data = 1.0;
	sscanf(buf,"%f",&data);
	return data;
}
