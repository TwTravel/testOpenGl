#ifndef  _PAGED_ARRAY_H_ 
#define  _PAGED_ARRAY_H_ 

#define PAGEMASK 0X1F
#define PAGEMASK2 0XE0
#define SHIFT 5
#include <windows.h>

struct CPAGE
{
	unsigned char _v[32][32];
};
struct FPAGE
{
	float _v[32][32];
};
class CCharArray  
{
public:
	int  GetSize(){return m_cells_x*m_cells_y*32*32;}
	void SetDataInfo(int x,int y,void* data);
	void  SetValue(int x,int y,unsigned char v);
	unsigned char& GetValue(int x, int y);
	void  Destory();
	BOOL  Create(int x,int y);
	CCharArray();
	virtual ~CCharArray();

	int m_x,m_y;
	int m_cells_x,m_cells_y;
	CPAGE* m_pages;
	
};

class CFloatArray
{
public:
	int    GetSize(){return m_cells_x*m_cells_y*32*32;}
	void   SetDataInfo(int x,int y,void* data);
	void   SetValue(int x,int y,float v);
	float& GetValue(int x, int y);
	void   Destory();
	BOOL   Create(int x,int y);
	CFloatArray();
	virtual ~CFloatArray();

	int m_x,m_y;
	int m_cells_x,m_cells_y;
	FPAGE* m_pages;
};

#endif 