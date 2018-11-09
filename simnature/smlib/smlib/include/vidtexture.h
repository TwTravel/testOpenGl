
#ifndef _VIDEO_TEXTURE_H_
#define _VIDEO_TEXTURE_H_
#include <dshow.h>
#include <streams.h>
#include <atlbase.h>
#include <windows.h>
#include <gl\gl.h>
#pragma comment(lib,"Strmiids.lib")
#pragma comment(lib,"Quartz.lib")

#ifdef _DEBUG
#pragma comment(lib,"Strmbasd.lib")
#else
#pragma comment(lib,"Strmbase.lib")
#endif

#pragma comment(lib,"Winmm.lib")

#include "image.h"
enum VIDEOTEX_TYPE
{
	VTT_REALSIZE,
	VTT_SCALESIZE,
	VTT_TRIMSIZE
};
typedef struct VideoImage
{
	IMAGESIZE _size;
	IMAGESIZE _raw_size;
	IMAGEDATA _data;
	void*     _privateData;

	VIDEOTEX_TYPE     _type;

	IGraphBuilder*  m_pGB;          // GraphBuilder
	IMediaControl*  m_pMC;          // Media Control
	IMediaPosition* m_pMP;          // Media Position
	IMediaEvent*    m_pME;          // Media Event


}VIDEOIMAGE,*PVIDEOIMAGE;

extern "C" int	   VIUpToPower2(int n);
extern "C" int     VIDownToPower2(int n);
extern "C" HRESULT CreateVideoImage(char* filename,VIDEOIMAGE* vpTexture);
extern "C" void    RetriveVImageInfo(VIDEOIMAGE* vpTexture);
extern "C" void    CheckMovieStatus(PVIDEOIMAGE vpTexture);
extern "C" void    ReleaseVideoImage(PVIDEOIMAGE vpTexture);
extern "C" BYTE*   GetVImageRawData(PVIDEOIMAGE vpTexture);
extern "C" GLuint  BuildVideoTexture(PVIDEOIMAGE vpTexture);
extern "C" void    UpdateVideoTexture(PVIDEOIMAGE vpTexture);
extern "C" void    SeekVideoPos(PVIDEOIMAGE vpImage,double second);
extern "C" void    PauseVideo(PVIDEOIMAGE vpImage);
extern "C" long    GetVideoEventCode(PVIDEOIMAGE vpImage);
extern "C" double  GetVideoLenght(PVIDEOIMAGE vpImage);
extern "C" int     IsVideoComplete(PVIDEOIMAGE vpImage);
extern "C" double  GetVideoRate(PVIDEOIMAGE vpImage);
extern "C" void    SetVideoRate(PVIDEOIMAGE vpImage,double rate);
extern "C" double  GetVideoPos(PVIDEOIMAGE vpImage);
extern "C" HRESULT RunVideo(PVIDEOIMAGE vpImage);
extern "C" HRESULT StopVideo(PVIDEOIMAGE vpImage);
#endif




