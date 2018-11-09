/***************************************************************
   ģ������:
       Configure.cpp
   Ŀ��: 
      �ṩ�������ļ��Ķ�ȡ

   ����:
        ������ HeartBlue
		       XpertSoft HeartBlue 2002/9/29
			   Allright reserved


   �����ļ��ֶΣ����ǰ������ļ����뵽������У�Ȼ�󰴷ֶ�
   �����棬���е���ϢΪ�ַ��������ǰ�����Ҫ���ַ�����ʽ��
   ����Ӧ����Ϣ
                    ������ 2003 - 4

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
         ����һ�������ļ���
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
		//TRACE(NULL,"�����ļ�������","�����ļ�����",MB_OK);
		return FALSE;
	}
	while(!in.eof())
	{
		in.getline(buf,128);
		//��һ���εı�ǣ����ǰѵ�ǰ�Ķ�����Ϊ����������
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
�ڲ�������
�ж϶����һ���ַ����ǲ���һ��������
�����ĸ�ʽ��: [SectionName]
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
����һ��������Ŀ
section : �������������������Ķε�����
itemname: ���������
*****************************************************/
BOOL CConfigure::InsertItem(char *section, char *itemName, char *data)
{
	int sec_count = m_sections.size();

	for(int i = 0; i<sec_count ;i++)
	{
		//�ҵ���һ���Ѿ����˵���
		if( strcmp(m_sections[i]._name,section) == 0)
		{
			for(int j = 0;j<m_sections[i]._items.size();j++)
			{
				//�ҵ�һ���Ѿ����˵�һ��������Ŀ�����Ǿ��޸���
				if(strcmp(itemName,m_sections[i]._items[j]._name)==0)
				{
					delete[] m_sections[i]._items[j]._data;
					m_sections[i]._items[j]._data = new char[strlen(data)+1];

					strcpy(m_sections[i]._items[j]._data,data);
					return TRUE;
				}
			}
			//û���ҵ�
			CCfgItem newItem;
			newItem._data = new char[strlen(data)+1];

			strcpy(newItem._data,data);
			strcpy(newItem._name,itemName);
			
			m_sections[i]._items.push_back(newItem);
			return TRUE;
		}
	}
	//�Ҳ���ָ��������
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
���ڵ���Ŀ�ģ�����Ҫ����
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
�ͷ����е����ݺ����ݵĴ洢�ռ�
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
�������ļ�д�ص��ļ��У��������ǿ��Ա����ڳ�������ʱ
�����õ��޸�
****************************************************/
void CConfigure::Save()
{
	//�򵥵Ķ����ݽ���һ�α�������
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
���µĺ���Ϊ���ո����Ķ����������������ѯ��Ϣ��
���Ұѵõ�����Ϣ��ʽ��������Ҫ����Ϣ
���У�GetString�����ǹؼ�
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
