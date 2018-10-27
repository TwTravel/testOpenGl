

#ifndef C_24_BitMap
#define C_24_BitMap
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

 

#define IJT int i,j,t;
#define Loopi(k) for(i=0;i<k;i++)
#define Loopj(k) for(j=0;j<k;j++)

#define BOUND(x,a,b) (((x) < (a)) ? (a) : (((x) > (b)) ? (b) : (x)))
#define THRESHOLD(x,a,b,v) x<v?a:b

#define Re_Local_Mem(x,num) {delete []x;x=NULL;x=new BYTE[num];\
C24Memcnt+=num;}
#define YY(R,G,B) (R+16)*0.257+(G+128)*0.504+(B+128)*0.098
#define UU(R,G,B) (R+16)*-0.148+(G+128)*-0.291+(B+128)*0.439
#define VV(R,G,B) (R+16)*0.439+(G+128)*-0.368+(B+128)*-0.071

#define RGBcolor(tt) RGB(tt.rgbRed,tt.rgbGreen,tt.rgbBlue)

typedef struct tagRECT
{
    int    left;
    int    top;
    int    right;
    int    bottom;
} RECT,*PRECT;

typedef unsigned int       DWORD;
typedef int                 BOOL;
typedef unsigned short      WORD;
typedef int            LONG;
typedef unsigned char BYTE;

typedef struct tagBITMAPFILEHEADER {
	WORD bfType;
	DWORD bfSize;
	WORD bfReserved1;
	WORD bfReserved2;
	DWORD bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
	DWORD biSize;
	LONG  biWidth;
	LONG  biHeight;
	WORD  biPlanes;
	WORD  biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	LONG  biXPelsPerMeter;
	LONG  biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

BYTE BMP24FileHead[54]={
	0x42,0x4D,0x08,0x79,0x02,0x00,0x00,0x00,0x00,0x00,0x36,0x00,0x00,0x00,0x28,0x00,
		0x00,0x00,0xC8,0x00,0x00,0x00,0x0E,0x01,0x00,0x00,0x01,0x00,0x18,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x12,0x0B,0x00,0x00,0x12,0x0B,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00};
	
	struct C24PixVal
	{BYTE *r,*g,*b,R,G,B;};
	
//FileMngr FMG("out.txt");

//FileMngr FMG("out.txt");

class C24BitMap
{
public:
	BITMAPINFOHEADER BitMpInfoHead;BITMAPFILEHEADER BitMpHead;//图像信息头结构,图像头结构
	void inline Load(char*FileName);
	void inline Save(char*FileName);
	void inline FormatF(int w,int h);
	inline C24BitMap&operator=(const C24BitMap&OtherPic);//算符重载
	int Width,Height,LineWidth;
	BYTE *Buffer;
	inline C24BitMap();
	inline ~C24BitMap();
	int C24Memcnt;
	C24PixVal PenColor;
	int DotSize;
	int PicDataSize;
	void inline RectDot(int x,int y);
	void inline RectDot(int x,int y,int RR,int GG,int BB);
	void inline SigDot(int x,int y);
	void inline DrawCircle(int x, int y, double R, double tp);
	void inline DrawCircleLine(int x, int y, double R);
	void inline DrawLine(int x1,int y1,int x2,int y2);
	void inline DrawTkLine(int x1,int y1,int x2,int y2,double R);
	void inline DrawDashLine(int x1,int y1,int x2,int y2,double R);
	void inline CleanPic(int Color);

	void inline Rectangle(int centerX,int centerY , double dx, double dy,
		                 double x1,double y1,double x2,double y2);

	void inline RandPenColor();
	void Rectangle(int x1,int y1,int x2,int y2);
	void ClearPic(int val);
	int FillRect;
	C24PixVal BrushColor;
	C24PixVal GetPenColor(int x,int y);

	void SetColor(int val)
	{
		if(val==1)
		{ PenColor.R = 255;
		  PenColor.G = 0;
		  PenColor.B = 0;
		}
		 if(val==2)
		{ PenColor.R = 0;
		  PenColor.G = 0;
		  PenColor.B = 255;
		}
		 if(val==0)
		{ PenColor.R = 0;
		  PenColor.G = 255;
		  PenColor.B = 0;
		}
		
	};
	int  SaveToMem(BYTE*outbuf)
	{   
	  int cnt(0);
	  memcpy(&outbuf[cnt],(void*)&BitMpHead, 14);cnt+=14;
	  memcpy(&outbuf[cnt],(void*)&BitMpInfoHead,40);cnt+=40;
	  memcpy(&outbuf[cnt],Buffer,LineWidth*Height); cnt+=LineWidth*Height;
	  return cnt;
	} 

	void  SetPenColor(int R,int G,int B)
	{
		PenColor.R = R; PenColor.G = G; PenColor.B = B;
	}
	//int FillRec;
//----------------------------------
    BYTE *GetRowStartPt(int row)
	{
	  return Buffer+(Height-1-row)* LineWidth;
	}
};

inline C24PixVal get_pix_color(C24BitMap&pic,int x,int y);




C24PixVal C24BitMap::GetPenColor(int x,int y)
{
	C24PixVal Temp;
	Temp = get_pix_color(*this,x,y);
	Temp.R = *Temp.r;
	Temp.G = *Temp.g;
	Temp.B = *Temp.b; 
	return Temp;
}

void C24BitMap::ClearPic(int val=0)
{
int i;	
Loopi(LineWidth*Height) Buffer[i]= val;
}


void TransAxis(double &X,double&Y,double&dx,double&dy)
{
 double TmpX,TmpY;
 TmpX = X*dx    + Y*dy;
 TmpY = X*(-dy) + Y*dx;
 X = TmpX;
 Y = TmpY;
}

void inline C24BitMap::Rectangle(int centerX,int centerY , double dx, double dy,
		                       double x1,double y1,double x2,double y2)
{ 
	int i;
	double PtAxisX[4];
    double PtAxisY[4];

	dy = -dy;

	PtAxisX[0] = x1; PtAxisY[0] = y1; 
	PtAxisX[1] = x1; PtAxisY[1] = y2; 
	PtAxisX[2] = x2; PtAxisY[2] = y2; 
	PtAxisX[3] = x2; PtAxisY[3] = y1; 

	Loopi(4)
	{
		PtAxisX[i] -= centerX;
		PtAxisY[i] -= centerY;
		TransAxis(PtAxisX[i],PtAxisY[i], dx, dy);
	}
	//transX1 += centerX; transX2 += centerX ;
	//transY1 += centerY; transY2 += centerY; 
	Loopi(4)
	{
      PtAxisX[i] += centerX;
      PtAxisY[i] += centerY;
	}

 
	DrawTkLine(PtAxisX[0], PtAxisY[0], PtAxisX[1], PtAxisY[1],1.5); 
	DrawTkLine(PtAxisX[1], PtAxisY[1], PtAxisX[2], PtAxisY[2],1.5);
 	DrawTkLine(PtAxisX[2], PtAxisY[2], PtAxisX[3], PtAxisY[3],1.5); 
	DrawTkLine(PtAxisX[3], PtAxisY[3], PtAxisX[0], PtAxisY[0],1.5); 
}

void inline C24BitMap::Rectangle(int x1,int y1,int x2,int y2)
{
	x1=BOUND(x1,0,Width - 1);  x2 = BOUND(x2,0, Width -1);
    y1=BOUND(y1,0,Height- 1);  y2 = BOUND(y2,0, Height-1);

    DrawLine(x1,y1,x1,y2); DrawLine(x1,y1,x2,y1);
 	DrawLine(x2,y1,x2,y2); DrawLine(x1,y2,x2,y2); 

	if(FillRect==0)return;

	int i,j;
	C24PixVal Temp;
	
	for (i=x1;i<=x2;i++)
	{
		for (j=y1;j<=y2;j++)
		{
			Temp = get_pix_color(*this,i,j);
			//*Temp.r = *Temp.r*5/10 + BrushColor.R*5/10;
			//*Temp.g = *Temp.g*5/10 + BrushColor.G*5/10;
		//	*Temp.b = *Temp.b*5/10 + BrushColor.B*5/10;
			
			*Temp.r =  BrushColor.R ; *Temp.g =  BrushColor.G ; *Temp.b =  BrushColor.B ;
		}
	}

	//this->PenColor.R = 0; this->PenColor.G = 255; this->PenColor.B = 0;
	//this->RectDot((x1+x2)/2,(y1+y2)/2); 	/**/


}

void inline C24BitMap::RectDot(int x,int y,int RR,int GG,int BB)
{
   int i,j;
   C24PixVal Temp;
	for (i=x-DotSize;i<=x+DotSize;i++)
	{
		for (j=y-DotSize;j<=y+DotSize;j++)
		{
			Temp = get_pix_color(*this,i,j);
			*Temp.r = RR;
			*Temp.g = GG;
			*Temp.b = BB;
		}
	}
}

void inline C24BitMap::RandPenColor()
{
 PenColor.R = rand()%255;
 PenColor.G = rand()%255;
 PenColor.B = rand()%255; 
}

void inline C24BitMap::CleanPic(int Color=0)
{
 int i;
 PicDataSize = LineWidth *Height;
 Loopi(PicDataSize) Buffer[i] = Color;
}

void inline C24BitMap::DrawDashLine(int x1,int y1,int x2,int y2,double R)
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	int ux = ((dx > 0) << 1) - 1;//x的增量方向，取或-1
	int uy = ((dy > 0) << 1) - 1;//y的增量方向，取或-1
	int x = x1, y = y1, eps;//eps为累加误差
	
	double distance;
	
	eps = 0;dx = abs(dx); dy = abs(dy); 
	if (dx > dy) 
	{
		for (x = x1; x != x2; x += ux)
		{
			distance = sqrt(float(
				(x1-x)*(x1-x) + (y1-y)*(y1-y)
				));
            int distance_i = int(distance);
            if((distance_i%6)<3)
			   DrawCircle( x, y, R, 1.0);

			eps += dy;
			if ((eps << 1) >= dx)
			{
				y += uy; eps -= dx;
			}
		}
	}
	else
	{
		for (y = y1; y != y2; y += uy)
		{
			distance = sqrt(float(
				(x1-x)*(x1-x) + (y1-y)*(y1-y))
				);
            int distance_i = int(distance);
            if((distance_i%6)<3)
			   DrawCircle( x, y, R , 1.0);

			eps += dx;
			if ((eps << 1) >= dy)
			{
				x += ux; eps -= dy;
			}
		}
	}   
}


void inline C24BitMap::DrawTkLine(int x1,int y1,int x2,int y2,double R)
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	int ux = ((dx > 0) << 1) - 1;//x的增量方向，取或-1
	int uy = ((dy > 0) << 1) - 1;//y的增量方向，取或-1
	int x = x1, y = y1, eps;//eps为累加误差
	
	eps = 0;dx = abs(dx); dy = abs(dy); 
	if (dx > dy) 
	{
		for (x = x1; x != x2; x += ux)
		{
		    DrawCircle( x, y, R,1.0);
			eps += dy;
			if ((eps << 1) >= dx)
			{
				y += uy; eps -= dx;
			}
		}
	}
	else
	{
		for (y = y1; y != y2; y += uy)
		{
			DrawCircle( x, y, R,1.0);
			eps += dx;
			if ((eps << 1) >= dy)
			{
				x += ux; eps -= dy;
			}
		}
	}   

}

void inline C24BitMap::DrawLine(int x1,int y1,int x2,int y2)
{
	 
	
	int dx = x2 - x1;
	int dy = y2 - y1;
	int ux = ((dx > 0) << 1) - 1;//x的增量方向，取或-1
	int uy = ((dy > 0) << 1) - 1;//y的增量方向，取或-1
	int x = x1, y = y1, eps;//eps为累加误差
	
	eps = 0;dx = abs(dx); dy = abs(dy); 
	if (dx > dy) 
	{
		for (x = x1; x != x2; x += ux)
		{
			SigDot( x, y);
			eps += dy;
			if ((eps << 1) >= dx)
			{
				y += uy; eps -= dx;
			}
		}
	}
	else
	{
		for (y = y1; y != y2; y += uy)
		{
			SigDot( x, y);
			eps += dx;
			if ((eps << 1) >= dy)
			{
				x += ux; eps -= dy;
			}
		}
	}   
}


void inline C24BitMap::DrawCircle(int x, int y, double R, double tp =0.5)
{
   int i,j;
   for(i = x-R; i<= (x+R); i++)
	   for(j= y-R; j<= (y+R); j++)
	   {
         float newx,newy;
		 newx = i;  newy = j;
		 float distance = sqrt( ( newx - x) * (newx - x) +
			                    ( newy - y) * (newy - y) );
		 if(distance>R)continue;
		 C24PixVal Temp;
		 Temp = get_pix_color(*this,i,j);
		 
		 *Temp.r = float(PenColor.R) *tp + float(*Temp.r)*(1-tp);
		 *Temp.g = float(PenColor.G) *tp + float(*Temp.g)*(1-tp);
         *Temp.b = float(PenColor.B) *tp + float(*Temp.b)*(1-tp); 
	   }

};

void inline C24BitMap::DrawCircleLine(int x, int y, double R)
{
	int i,j;
	for(i = x-R; i<= (x+R); i++)
		for(j= y-R; j<= (y+R); j++)
		{
			float newx,newy;
			newx = i;  newy = j;
			float distance = sqrt( ( newx - x) * (newx - x) +
				( newy - y) * (newy - y) );
			if(fabs(distance-R)>1.5)
				                continue;
			C24PixVal Temp;
			Temp = get_pix_color(*this,i,j);
			*Temp.r = PenColor.R;
			*Temp.g = PenColor.G;
			*Temp.b = PenColor.B; 
	   }
}

void inline C24BitMap::SigDot(int x,int y)
{
 C24PixVal Temp;
 Temp = get_pix_color(*this,x,y);
 *Temp.r = PenColor.R;
 *Temp.g = PenColor.G;
 *Temp.b = PenColor.B; 
}

void inline C24BitMap::RectDot(int x,int y)
{
 int i,j;
 C24PixVal Temp;
 for (i=x-DotSize;i<=x+DotSize;i++)
 {
	 for (j=y-DotSize;j<=y+DotSize;j++)
	 {
		 Temp = get_pix_color(*this,i,j);
		 *Temp.r = PenColor.R;
		 *Temp.g = PenColor.G;
		 *Temp.b = PenColor.B;
	 }
 }
}

inline C24BitMap& C24BitMap::operator=(const C24BitMap&OtherPic)//算符重载
{int i;
FormatF(OtherPic.Width,OtherPic.Height);
int BtCnt(Height*LineWidth);
Loopi(BtCnt)Buffer[i]=OtherPic.Buffer[i];
return *this;}

C24BitMap::C24BitMap()
{
	Buffer=NULL;LineWidth=0;Height=0;
	PicDataSize = 0;
	PenColor.R = 255;
	PenColor.G = 0;
	PenColor.B = 0;
	DotSize = 2;
};

C24BitMap::~C24BitMap()
{
	delete []Buffer;Buffer=NULL;
}

inline void C24BitMap::FormatF(int w,int h)
{ 
BYTE BMP24FileHead[54]={
	0x42,0x4D,0x08,0x79,0x02,0x00,0x00,0x00,0x00,0x00,0x36,0x00,0x00,0x00,0x28,0x00,
		0x00,0x00,0xC8,0x00,0x00,0x00,0x0E,0x01,0x00,0x00,0x01,0x00,0x18,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x12,0x0B,0x00,0x00,0x12,0x0B,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00};
	
	C24Memcnt-=LineWidth*Height;
	memcpy((void*)&BitMpHead,(void*)BMP24FileHead,14);
	memcpy((void*)&BitMpInfoHead,(void*)(BMP24FileHead+14),40);
	BitMpInfoHead.biHeight=h;BitMpInfoHead.biWidth=w;
	Width = w; Height = h; LineWidth = 4*int((Width*3+3)/4);//每行数据字节大小
	PicDataSize = Height * LineWidth;
	Re_Local_Mem(Buffer,LineWidth*Height);
}

inline void C24BitMap::Save(char*FileName)
{
	FILE*file = fopen(FileName,"wb+");
	fwrite((void*)&BitMpHead,1,14,file);
	fwrite(&BitMpInfoHead,1,sizeof(BitMpInfoHead),file);
	fwrite((void*)Buffer,1,(LineWidth*Height),file);
    fclose(file);
}

inline void C24BitMap::Load(char*FileName)
{
	C24Memcnt-=LineWidth*Height;
	///int handle=open(FileName,
	//				O_RDONLY | O_BINARY, S_IWRITE | S_IREAD);
	FILE*file = fopen(FileName,"rb");
	
	fread((void*)&BitMpHead,1,14,file);
	fread((char*)&BitMpInfoHead,1,40,file);
	
	//printf("%i,%i\n",sizeof(BitMpHead),sizeof(BitMpInfoHead));
	
	Height = BitMpInfoHead.biHeight;
	Width  = BitMpInfoHead.biWidth;
	
	if(Height<0)
	{
		Height = -Height;
		BitMpInfoHead.biHeight= Height;
	}
	LineWidth=4*int((Width*3+3)/4);//每行数据字节大小
	Re_Local_Mem(Buffer,LineWidth*Height);
	fread((void*)Buffer,1,(LineWidth*Height),file);
    fclose(file);
}


inline C24PixVal get_pix_color(C24BitMap&pic,int x,int y)
{x=BOUND(x,0,pic.Width-1);y=BOUND(y,0,pic.Height-1);
C24PixVal Result;
Result.b=pic.Buffer+(pic.Height-1-y)*pic.LineWidth+x*3;
Result.g=Result.b+1;Result.r=Result.b+2;

//int BB=int(pic.Buffer+pic.Height*pic.LineWidth-1);
//if(int(Result.g)>BB)memcpy(&Result.g,&BB,4);
//if(int(Result.r)>BB)memcpy(&Result.r,&BB,4);
//if(int(Result.b)>BB)memcpy(&Result.b,&BB,4);

return Result;}

C24PixVal inline C24PtItrp(C24BitMap&Pix,int x,int y,float fu,float fv)
{int i,j;
C24PixVal abyDot[2][2],Result; //像素点(x, y)的数据位置

Loopi(2)Loopj(2)abyDot[i][j]=get_pix_color(Pix,x+i,y+j);

Result.R=(BYTE)( BOUND(
				(1-fu)*(1-fv)*(float)*abyDot[0][0].r+ 
				(1-fu)*(  fv)*(float)*abyDot[0][1].r+ 
				(  fu)*(1-fv)*(float)*abyDot[1][0].r+
				(  fu)*(  fv)*(float)*abyDot[1][1].r,0,255 )  );

Result.G=(BYTE)( BOUND(
				(1-fu)*(1-fv)*(float)*abyDot[0][0].g+ 
				(1-fu)*(  fv)*(float)*abyDot[0][1].g+ 
				(  fu)*(1-fv)*(float)*abyDot[1][0].g+
				(  fu)*(  fv)*(float)*abyDot[1][1].g,0,255 )  );

Result.B=(BYTE)( BOUND(
				(1-fu)*(1-fv)*(float)*abyDot[0][0].b+ 
				(1-fu)*(  fv)*(float)*abyDot[0][1].b+ 
				(  fu)*(1-fv)*(float)*abyDot[1][0].b+
				(  fu)*(  fv)*(float)*abyDot[1][1].b,0,255 )  );

return Result;
}

inline C24PixVal C24PtItrp_A(C24BitMap&Pix,float fx,float fy)
{
	int x=int(fx);
	int y=int(fy);
	float fu=fx-float(x);
	float fv=fy-float(y);
	return C24PtItrp(Pix,x,y,fu,fv);
}

inline C24PixVal C24PtItrp(BYTE*PtPos,int x,int y,float fu,float fv,
						   int nScanWidth,int nScanHeight)
{
	C24PixVal Result;
	//相邻的四个像素最右下角点的x, y坐标偏移量
	int nx(1),ny(1);
	if((x+1)>(nScanWidth-1))nx=0;if(y==0)ny=0;
	
	C24PixVal abyDot[2][2]; //像素点(x, y)的数据位置
	BYTE* pbySrc=PtPos;//获取像素数值(x, y) = (x, y) + (0, 0)						
	
	abyDot[0][0].R=PtPos[0];abyDot[0][0].G=PtPos[1];abyDot[0][0].B=PtPos[2];
	
	pbySrc=PtPos+nx*3;//(x+1,y)=(x,y)+(1,0)
	abyDot[1][0].R=PtPos[0];abyDot[1][0].G=PtPos[1];abyDot[1][0].B=PtPos[2];
	
	pbySrc=(PtPos-ny*nScanWidth);//指向下一行数据
	abyDot[0][1].R=PtPos[0];abyDot[0][1].G=PtPos[1];abyDot[0][1].B=PtPos[2];
	
	pbySrc=pbySrc+nx*3;
	abyDot[1][1].R=PtPos[0];abyDot[1][1].G=PtPos[1];abyDot[1][1].B=PtPos[2];
	
	Result.R=(BYTE)( BOUND(
		(1-fu)*(1-fv)*(float)abyDot[0][0].R+ 
		(1-fu)*(  fv)*(float)abyDot[0][1].R+ 
		(  fu)*(1-fv)*(float)abyDot[1][0].R+
		(  fu)*(  fv)*(float)abyDot[1][1].R,0,255 )  );
	
	Result.G=(BYTE)( BOUND(
		(1-fu)*(1-fv)*(float)abyDot[0][0].G+ 
		(1-fu)*(  fv)*(float)abyDot[0][1].G+ 
		(  fu)*(1-fv)*(float)abyDot[1][0].G+
		(  fu)*(  fv)*(float)abyDot[1][1].G,0,255 )  );
	
	Result.B=(BYTE)( BOUND(
		(1-fu)*(1-fv)*(float)abyDot[0][0].B+ 
		(1-fu)*(  fv)*(float)abyDot[0][1].B+ 
		(  fu)*(1-fv)*(float)abyDot[1][0].B+
		(  fu)*(  fv)*(float)abyDot[1][1].B,0,255 )  );
	
	return Result;
}



void inline DrawRectangle(C24BitMap&Pic,RECT&Rt)
{
	int i,j;
	C24BitMap Cpy;
	Cpy = Pic;
	int Cnt;
	Cnt = Pic.Height*Cpy.LineWidth;
	C24PixVal Tp1,Tp2;
	
	Loopi(Cnt)
	{
		int Color = Pic.Buffer[i]*8/10;
		Pic.Buffer[i] = Color;
	}
	
	for(i=Rt.left;i<Rt.right;i++)
	{
		for(j=Rt.top;j<Rt.bottom;j++)
		{
			
			Tp1 = get_pix_color(Pic,i,j);
			Tp2 = get_pix_color(Cpy,i,j);
			*Tp1.r = *Tp2.r;
			*Tp1.g = *Tp2.g;
			*Tp1.b = *Tp2.b;
		}
		C24PixVal Tp1;
		Tp1 = get_pix_color(Pic,i,Rt.top);
		*Tp1.r = 255; *Tp1.g =0; *Tp1.b = 0;
		Tp1 = get_pix_color(Pic,i,Rt.bottom);
		*Tp1.r = 255; *Tp1.g =0; *Tp1.b = 0;  
	}
	
	for(j=Rt.top;j<Rt.bottom;j++)
	{
		Tp1 = get_pix_color(Pic,Rt.left,j);
		*Tp1.r = 255; *Tp1.g =0; *Tp1.b = 0;
		Tp1 = get_pix_color(Pic,Rt.right,j);
		*Tp1.r = 255; *Tp1.g =0; *Tp1.b = 0;  
	}
	
}
#endif
