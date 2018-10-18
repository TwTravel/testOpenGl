//****************************************************************************
//  Dib.h: interface for the CDib class.
//
//  Copyright (c) Boris J. Wang (e_boris2002@hotmail.com) 2002 - 2003
//  From Institute of Computing Technology, Chinese Academy of Sciences
//                                                Beijing 100871, China
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
//  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
//  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
//  PARTICULAR PURPOSE.
//****************************************************************************

#if !defined(AFX_DIB_H__6C294E63_FE8B_11D0_9B6B_444553540000__INCLUDED_)
#define AFX_DIB_H__6C294E63_FE8B_11D0_9B6B_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

////////////////////////////////////////////////////////////////////
//Disable it for loading texture for file or loading it for resource
#define DIB_LOAD_FILE_FROM_RESOURCE
////////////////////////////////////////////////////////////////////

#ifdef DIB_LOAD_FILE_FROM_RESOURCE

class CDib
{
protected:
	LPBYTE	lpBits;
	int cx,cy;
public:
	CDib(){lpBits = NULL;}
	~CDib();
	UINT GetDibHeight();
	UINT GetDibWidth();
	BYTE* GetDibBitsPtr();
	BOOL LoadBitmapFile(const char* fileName);
};

#else

class CDib
{
public:
	BYTE* GetDibBitsPtr();
	LPBITMAPINFO GetDIbInfoPtr();
	LPBITMAPINFOHEADER GetDibInfoHeaderPtr();
	UINT GetDibHeight();
	UINT GetDibWidth();
	DWORD GetDibSizeImage();
	CDib();
	BOOL LoadBitmapFile(const char* fileName);
	CDib(const char* fileName);
	virtual ~CDib();

protected:
	LPBITMAPFILEHEADER	m_pBmFileHeader;
	LPBITMAPINFO		m_pBmInfo;
	LPBITMAPINFOHEADER	m_pBmInfoHeader;
	RGBQUAD*			m_pRGBTable;
	BYTE*				m_pDibBits;
	UINT				m_numColors;
};

#endif



#endif // !defined(AFX_DIB_H__6C294E63_FE8B_11D0_9B6B_444553540000__INCLUDED_)
