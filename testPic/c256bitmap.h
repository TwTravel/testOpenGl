
#ifndef c256bitmap_Class
#define c256bitmap_Class
#include <stdio.h>
#include <fcntl.h>
typedef unsigned char BYTE;


#define IJT   int i,j,t;
#define Loopi(k) for(i=0;i<k;i++)
#define LoopPic_ij(Pic) Loopi(Pic.Width)Loopj(Pic.Height)
#define BOUND(x,a,b) (((x) < (a)) ? (a) : (((x) > (b)) ? (b) : (x)))
#define THRESHOLD(x,a,b,v) x<v?a:b

//#define max(a,b) ((a) > (b) ? (a) : (b)) 
//#define min(a,b) ((a) < (b) ? (a) : (b)) 


#define RGBcolor(tt) RGB(tt.rgbRed,tt.rgbGreen,tt.rgbBlue)
BYTE BMPFileHead[54]={
0x42,0x4D,0xC6,0x2C,0x00,0x00,0x00,0x00,0x00,0x00,0x36,0x04,0x00,0x00,0x28,0x00,
0x00,0x00,0x58,0x00,0x00,0x00,0x76,0x00,0x00,0x00,0x01,0x00,0x08,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x00,0x00,0x00,0x01,0x00,0x00};

typedef struct tagRGBQUAD {
  BYTE rgbBlue;
  BYTE rgbGreen;
  BYTE rgbRed;
  BYTE rgbReserved;
} RGBQUAD;/**/

class C256BitMap
{
public:
C256BitMap();
~C256BitMap();
BITMAPINFOHEADER BitMpInfoHead;BITMAPFILEHEADER BitMpHead;//图像信息头结构,图像头结构
RGBQUAD ColorTable[256];//色彩指针
C256BitMap&operator=(const C256BitMap&OtherPic);//算符重载
void Load(char*FileName);
void Save(char*FileName);
void Clear();
void FormatF(int w,int h);
int Width,Height,LineWidth;
BYTE *Buffer;
static int C256Memcnt;
};
int C256BitMap::C256Memcnt=0;
C256BitMap::~C256BitMap()
{C256Memcnt-=LineWidth*Height;
delete []Buffer;Buffer=NULL;
}

void C256BitMap::Clear()
{int i;
 Loopi(Height* LineWidth) Buffer[i]=255; 
}

C256BitMap& C256BitMap::operator=(const C256BitMap&OtherPic)//算符重载
{int i;
FormatF(OtherPic.Width,OtherPic.Height);
int BtCnt(Height*LineWidth);
Loopi(BtCnt)Buffer[i]=OtherPic.Buffer[i];
return *this;}

C256BitMap::C256BitMap()
{LineWidth=0;Height=0;Width=0;
Buffer=NULL;}

void C256BitMap::FormatF(int w,int h)
{int i;
C256Memcnt-=LineWidth*Height;
memcpy((void*)&BitMpHead,(void*)BMPFileHead,14);
memcpy((void*)&BitMpInfoHead,(void*)(BMPFileHead+14),40);
BitMpInfoHead.biHeight=h;BitMpInfoHead.biWidth=w;
Width=w;Height=h;LineWidth=4*int((Width+3)/4);//每行数据字节大小
delete []Buffer;Buffer=NULL;Buffer=new BYTE[LineWidth*Height];
C256Memcnt+=LineWidth*Height;

Loopi(256)
{ColorTable[i].rgbRed=i;
ColorTable[i].rgbGreen=i;
ColorTable[i].rgbBlue=i;}
}

void C256BitMap::Save(char*FileName)
{
 
FILE*file = fopen(FileName,"wb+");
fwrite((void*)&BitMpHead,1,14,file);
fwrite(&BitMpInfoHead,1,sizeof(BitMpInfoHead),file);
fwrite((void*)ColorTable, sizeof(RGBQUAD),256,file);
fwrite((void*)Buffer,1,(LineWidth*Height),file);
fclose(file);

}

void C256BitMap::Load(char*FileName)
{
FILE*file = fopen(FileName,"rb");
fread((void*)&BitMpHead,1,14,file);
fread(&BitMpInfoHead,1,sizeof(BitMpInfoHead),file);
fread((void*)ColorTable, sizeof(RGBQUAD),256,file);

Height=BitMpInfoHead.biHeight;Width=BitMpInfoHead.biWidth;
LineWidth=4*int((Width+3)/4);//每行数据字节大小
delete []Buffer;Buffer=NULL;Buffer=new BYTE[LineWidth*Height];
fread((void*)Buffer,1,(LineWidth*Height),file);
fclose(file);

}

/*void Disp(TCanvas*CV,C256BitMap*Pic,int x=0,int y=0)
{
int i,j,V;Loopi(Pic->Width)Loopj(Pic->Height)
{V=Pic->PicPt[j][i];
CV->Pixels[x+i][y+j]=RGBcolor(Pic->ColorTable[V]);
}
} */

inline BYTE* get_pix_color(C256BitMap&pic,int x,int y)
{x=BOUND(x,0,pic.Width-1);y=BOUND(y,0,pic.Height-1);
return pic.Buffer+(pic.Height-1-y)*pic.LineWidth+x;}

#endif
