//****************************************************************************
//  Dib.cpp: implementation of the CDib class.
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

#include "stdafx.h"
#include "Dib.h"
#include "windowsX.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifdef DIB_LOAD_FILE_FROM_RESOURCE

BOOL CDib::LoadBitmapFile(const char* ResName)
{
	ASSERT(ResName);
	ASSERT(!lpBits);

	HBITMAP Bitmap;
	Bitmap = (HBITMAP)LoadImage(AfxGetApp()->m_hInstance,ResName,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION|LR_DEFAULTSIZE);

	if(Bitmap)
	{BITMAP bm;

		if(GetObject(Bitmap,sizeof(BITMAP),&bm))
		{
			cx = bm.bmWidth;
			cy = bm.bmHeight;

			lpBits = new BYTE[3*cx*cy];
			if(lpBits)
			{
				GetBitmapBits(Bitmap,3*cx*cy,lpBits);
				DeleteObject(Bitmap);
				return TRUE;
			}
		}

		DWORD d=GetLastError();

		DeleteObject(Bitmap);
		return FALSE;
	}
	else
		return FALSE;
}

UINT CDib::GetDibWidth()
{
	ASSERT(lpBits);
	return cx;
}

UINT CDib::GetDibHeight()
{
	ASSERT(lpBits);
	return cy;
}

BYTE* CDib::GetDibBitsPtr()
{
	ASSERT(lpBits);
	return lpBits;
}

CDib::~CDib()
{
	if(lpBits)
	{
		delete [] lpBits;
		lpBits = NULL;
	}
}

#else

CDib::CDib()
{
	m_pBmFileHeader=NULL;
	m_pBmInfo=NULL;
	m_pBmInfoHeader=NULL;
	m_pRGBTable=0;
	m_pDibBits=0;
}

CDib::CDib(const char* fileName)
{
	m_pBmInfo = NULL;
	LoadBitmapFile(fileName);
}

CDib::~CDib()
{
	if(m_pBmInfo)delete [] ((BYTE *)m_pBmInfo);
}

BOOL CDib::LoadBitmapFile(const char* fileName)
{
	CFile file;

	ASSERT(!m_pBmInfo);
	ASSERT(fileName);

	if(!file.Open(fileName,CFile::modeRead|CFile::shareDenyNone))return FALSE;

	BYTE* pDib = NULL;

	try
	{
		//Load Header
		BITMAPFILEHEADER bmFileHeader;
		file.Read((void*)&bmFileHeader,sizeof(bmFileHeader));

		//Check signture
		if(bmFileHeader.bfType!=0x4d42)
		{
			m_pBmFileHeader=0;
			m_pBmInfo=0;
			m_pBmInfoHeader=0;
			m_pRGBTable=0;
			m_pDibBits=0;
			m_numColors=0;
			return FALSE;
		}

		DWORD fileLength=file.GetLength();
		DWORD dibSize=fileLength-sizeof(bmFileHeader);
		
		pDib=new BYTE[dibSize];

		file.Read(pDib,dibSize);
		file.Close();

		m_pBmInfo=(LPBITMAPINFO)pDib;
		m_pBmInfoHeader=(LPBITMAPINFOHEADER)pDib;
		
		m_pBmInfoHeader->biSizeImage=GetDibSizeImage();
		m_pDibBits=pDib+m_pBmInfoHeader->biSize;
	}
	catch(CFileException * e)
	{
		e->Delete();
		if(pDib)delete [] pDib;
		return FALSE;
	}

	return TRUE;
}

DWORD CDib::GetDibSizeImage()
{
	ASSERT(m_pBmInfoHeader);

	if(m_pBmInfoHeader->biSizeImage==0)
	{
		//Width
		DWORD byteWidth=(DWORD)GetDibWidth();

		//Height
		DWORD height=(DWORD)GetDibHeight();

		DWORD imageSize=byteWidth*height;

		return imageSize;
	}
	else 
		return m_pBmInfoHeader->biSizeImage;
}

UINT CDib::GetDibWidth()
{
	ASSERT(m_pBmInfoHeader);
	return (UINT)m_pBmInfoHeader->biWidth;
}

UINT CDib::GetDibHeight()
{
	ASSERT(m_pBmInfoHeader);
	return (UINT)m_pBmInfoHeader->biHeight;
}

LPBITMAPINFOHEADER CDib::GetDibInfoHeaderPtr()
{
	ASSERT(m_pBmInfoHeader);
	return m_pBmInfoHeader;
}

LPBITMAPINFO CDib::GetDIbInfoPtr()
{
	ASSERT(m_pBmInfo);
	return m_pBmInfo;
}


BYTE* CDib::GetDibBitsPtr()
{
	ASSERT(m_pDibBits);
	return m_pDibBits;
}

#endif
