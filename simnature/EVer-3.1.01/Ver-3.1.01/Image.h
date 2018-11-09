/**********************************************************
  RenderPro Ver1.0.0
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
#ifndef _IMAGE_H
#define _IMAGE_H
#ifdef _DEBUG
#pragma comment(lib,"imaged.lib")
#else
#pragma comment(lib,"image.lib")
#endif


typedef struct ImageSize
{
	int width,height;
	int bits;
}IMAGESIZE,*PIMAGESIZE;

typedef unsigned char* IMAGEDATA;
#include <windows.h>

extern "C" int               Round4Byte(int n);
extern "C" int               IsPowerOf2(int n);
extern "C" void              FreeImageData(unsigned char* pData);

//For Bitmap format
extern "C" unsigned char*    LoadWinBitmap(char* file_name,PIMAGESIZE psize);
extern "C" unsigned char*    LoadWinBitmapRGB(char* filename,PIMAGESIZE psize);
extern "C" unsigned char*    ConvertBitmapTo32(unsigned char* pdata,IMAGESIZE size);
extern "C" int               ConvertRGBToBGR(unsigned char* image,IMAGESIZE size);
extern "C" int               ConvertBGRToRGB(unsigned char* image,IMAGESIZE size);
extern "C" void              SaveBitmap(char* filename,IMAGESIZE size,unsigned char* image);

//For Blank Image manipulate
extern "C" unsigned char*	 LoadBlankImage(IMAGESIZE size);
extern "C" int				 SetImagePixel(unsigned char* image,int x,int y,IMAGESIZE size,unsigned char cl[4]);
extern "C" int				 GetImagePixel(unsigned char* image,int x,int y,IMAGESIZE size,unsigned char cl[4]);
extern "C" void				 SetKeyColorAlpha(unsigned char* image,IMAGESIZE size,unsigned char cl[4],int r);
 
//For Jpeg de/comperess
extern "C" unsigned char*    DecompJPEG(char* file_name,int* x,int* y);
extern "C" void              FreeJPEGData(unsigned char* pData);
extern "C" int               CompressJPEG(char * filename,unsigned char *image_buffer,int image_height,int image_width, int quality);
extern "C" int               SaveRGBToJPEG(char* filename,IMAGESIZE size,unsigned char* image,int qulity);
extern "C" int               SaveBGRToJPEG(char* filename,IMAGESIZE size,unsigned char* image,int qulity);
extern "C" unsigned char*    LoadJPEG32(char* file_name,PIMAGESIZE psize);
extern "C" unsigned char*    LoadJPEG24(char* file_name,PIMAGESIZE psize);

//For OLE Picture Loader
extern "C" unsigned char*	 LoadPicture(char *szPathName, IMAGESIZE* psize);
extern "C" unsigned char*	 LoadPictureAsTexture(char *szPathName, IMAGESIZE* psize,long MaxTexDim);
extern "C" unsigned char*	 LoadPicture32(char *szPathName, IMAGESIZE* psize);
extern "C" unsigned char*	 LoadPictureAsTexture32(char *szPathName, IMAGESIZE* psize,long MaxTexDim);

//For Snap Windows
extern "C"  unsigned char*	 SnapWindow(HWND hWnd,IMAGESIZE* psize);
extern "C"  unsigned char*	 SnapWindowAsTexture(HWND hWnd,IMAGESIZE* psize,int MaxTexDim);


#endif