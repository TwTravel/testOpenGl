#include "PageArray.h"

CCharArray::CCharArray()
{

}

CCharArray::~CCharArray()
{

}

BOOL CCharArray::Create(int x, int y)
{
	m_cells_x = (x)/32;
	m_cells_y = (y)/32;
	if(x%32)
		m_cells_x ++;
	if(y%32)
		m_cells_y ++;
	m_x = x;
	m_y = y;
	m_pages = new CPAGE[m_cells_x * m_cells_y];
	return TRUE;
}

void CCharArray::Destory()
{
	delete []m_pages;
}

unsigned char& CCharArray::GetValue(int x, int y)
{
	int cx = x/32;
	int cy = y/32;
	int dx = x%32;
	int dy = y%32;

	return m_pages[cx + cy * m_cells_x]._v[dx][dy];
}

void CCharArray::SetValue(int x, int y, unsigned char v)
{
	int cx = x/32;
	int cy = y/32;
	int dx = x%32;
	int dy = y%32;

	 m_pages[cx + cy * m_cells_x]._v[dx][dy] = v;
}

void CCharArray::SetDataInfo(int x, int y, void *data)
{
	m_pages = (CPAGE*) data;
	m_cells_x = (x)/32;
	m_cells_y = (y)/32;
	if(x%32)
		m_cells_x ++;
	if(y%32)
		m_cells_y ++;
	m_x = x;
	m_y = y;
}

CFloatArray::CFloatArray(void)
{
}

CFloatArray::~CFloatArray(void)
{
}

BOOL CFloatArray::Create(int x, int y)
{
	m_cells_x = (x)/32;
	m_cells_y = (y)/32;
	if(x%32)
		m_cells_x ++;
	if(y%32)
		m_cells_y ++;
	m_x = x;
	m_y = y;
	m_pages = new FPAGE[m_cells_x * m_cells_y];
	return TRUE;
}

void CFloatArray::Destory()
{
	delete []m_pages;
}

float& CFloatArray::GetValue(int x, int y)
{
	int cx = x/32;
	int cy = y/32;
	int dx = x%32;
	int dy = y%32;

	return m_pages[cx + cy * m_cells_x]._v[dx][dy];
}

void CFloatArray::SetValue(int x, int y, float v)
{
	int cx = x/32;
	int cy = y/32;
	int dx = x%32;
	int dy = y%32;

	m_pages[cx + cy * m_cells_x]._v[dx][dy] = v;
}

void CFloatArray::SetDataInfo(int x, int y, void *data)
{
	m_pages = (FPAGE*) data;
	m_cells_x = (x)/32;
	m_cells_y = (y)/32;
	if(x%32)
		m_cells_x ++;
	if(y%32)
		m_cells_y ++;
	m_x = x;
	m_y = y;
}