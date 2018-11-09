
#ifndef _VIDEO_TEXTURE_H_
#define _VIDEO_TEXTURE_H_
#include <dshow.h>
#include <windows.h>
#include <gl\gl.h>


	#ifdef _DEBUG
	#pragma comment(lib,"videotexd.lib")
	#else
	#pragma comment(lib,"videotex.lib")
	#endif




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
extern "C" HRESULT CreateVideoImage(char* filename,VIDEOIMAGE* vpImage);
extern "C" void    RetriveVImageInfo(VIDEOIMAGE* vpImage);
extern "C" void    CheckMovieStatus(PVIDEOIMAGE vpImage);
extern "C" void    ReleaseVideoImage(PVIDEOIMAGE vpImage);
extern "C" BYTE*   GetVImageRawData(PVIDEOIMAGE vpImage);
extern "C" GLuint  BuildVideoTexture(PVIDEOIMAGE vpImage);
extern "C" void    UpdateVideoTexture(PVIDEOIMAGE vpImage);
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




