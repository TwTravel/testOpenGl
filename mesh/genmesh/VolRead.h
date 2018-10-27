#ifndef Voxel_Read_H
#define Voxel_Read_H
#include <string>
#include <math.h>
using namespace std;
#include "c256bitmap.h"
#define Loopi(k) for(i=0;i<k;i++)
#define Loopj(k) for(j=0;j<k;j++)
#define Loopt(k) for(t=0;t<k;t++)


struct POINT3DID {
	int newID;
	float x, y, z;
	int xi,yi,zi;
	double Val;
};

POINT3DID Mul2Pts(POINT3DID&Pt1,POINT3DID&Pt2)
{
POINT3DID ResPt;
ResPt.x = (Pt1.y*Pt2.z - Pt1.z*Pt2.y);
ResPt.y = (Pt1.z*Pt2.x - Pt1.x*Pt2.z);
ResPt.z = (Pt1.x*Pt2.y - Pt1.y*Pt2.x);  
return  ResPt;
}

POINT3DID FacMulPt(POINT3DID&Pt,double Factor)
{
POINT3DID ResPt;                         
ResPt.x = Pt.x * Factor;   
ResPt.y = Pt.y * Factor;   
ResPt.z = Pt.z * Factor;   
return  ResPt;                           	
}

double PtVecLen(POINT3DID&Pt)
{
 double dis;
 dis = Pt.x*Pt.x+Pt.y*Pt.y+Pt.z*Pt.z;
 dis = sqrt(dis);
 return dis;
}

POINT3DID Add2Pts(POINT3DID&Pt1,POINT3DID&Pt2)
{
POINT3DID ResPt;
ResPt.x = (Pt1.x + Pt2.x);
ResPt.y = (Pt1.y + Pt2.y);
ResPt.z = (Pt1.z + Pt2.z);  
return  ResPt;
}

POINT3DID Sub2Pts(POINT3DID&Pt1,POINT3DID&Pt2)   
{                                                
POINT3DID ResPt;                                 
ResPt.x = (Pt1.x - Pt2.x);                       
ResPt.y = (Pt1.y - Pt2.y);                       
ResPt.z = (Pt1.z - Pt2.z);                       
return  ResPt;                                   
}                                                

void NormalizePt(POINT3DID&Pt)
{
 double VecSum;
 VecSum  = sqrt(Pt.x*Pt.x+Pt.y*Pt.y+Pt.z*Pt.z);

 Pt.x = Pt.x/VecSum;
 Pt.y = Pt.y/VecSum;
 Pt.z = Pt.z/VecSum;
}

POINT3DID NormalizePt_(POINT3DID&Pt)
{
 POINT3DID ResPt;  
 double VecSum;
 VecSum  = sqrt(Pt.x*Pt.x+Pt.y*Pt.y+Pt.z*Pt.z);

 ResPt.x = Pt.x/VecSum;
 ResPt.y = Pt.y/VecSum;
 ResPt.z = Pt.z/VecSum;
 return ResPt;
}

struct PointVal
{
int x,y;
unsigned short val;
unsigned short*address;
int OutSideFlag;
};

int operator<(const PointVal & P1,const PointVal & P2)
{
	if(P1.val > P2.val )
		return 1;
	else
	    return 0;
}

void Getxyzvalue(char*str,double&xx,double&yy,double&zz)
{//DATE-OBS= '2006-12-23' 
 string temp,val;
 int pos;
 temp=str;
 pos=temp.find(" ");val=temp.substr(0,pos); 
 xx=atof(val.c_str());
 temp=temp.substr(pos+1,temp.length()); 
 pos=temp.find(" ");val=temp.substr(0,pos); 
 yy=atof(val.c_str());
 temp=temp.substr(pos+1,temp.length()); 
 zz=atof(temp.c_str());
 
}

void read_resolution(char*prefix,double&xx,double&yy,double&zz,double*ImgResl)
{
 string str;
str=prefix;
str+=".hdr";
FILE*file;
file=fopen(str.c_str(),"r");
char Buffer[100];
//fgets(Buffer, 100, file);
fgets(Buffer, 100, file);

Getxyzvalue(Buffer, xx, yy, zz);
fgets(Buffer, 100, file);
Getxyzvalue(Buffer, ImgResl[0],ImgResl[1], ImgResl[2]);

fclose(file);
}

class CVolumeData
{
public:
CVolumeData();
~CVolumeData();
int ImWidth,ImHeight,VolHeight;
double ImgResl[3];
void Load(char*FileName);
void Save(char*FileName);
void Clear();
void RemoveCircle(int r);
void FormatF(int w,int h,int sliceNum);
void FormatF(CVolumeData*Vol,int filldata);
void Range(int min,int max);
void DumpOneSlice(int z);
unsigned short*Buffer;
};

void CVolumeData::FormatF(CVolumeData*Vol,int filldata=0)
{
	 FormatF(Vol->ImWidth,Vol->ImHeight,Vol->VolHeight);   
     memcpy(ImgResl,Vol->ImgResl,sizeof(double)*3);
	if(filldata)
		memcpy(Buffer,Vol->Buffer,2* Vol->ImWidth*Vol->ImHeight*Vol->VolHeight);
	else
		Clear();
}

unsigned short* get_voxl_color(CVolumeData&Vol,int x,int y,int z)
{
  x = BOUND(x,0,Vol.ImWidth  -1);
  y = BOUND(y,0,Vol.ImHeight -1);
  z = BOUND(z,0,Vol.VolHeight-1);
  int slicecnt;
  slicecnt = Vol.ImWidth*Vol.ImHeight;
 
return Vol.Buffer+slicecnt*z+y*Vol.ImWidth+x;
 
}

void  CVolumeData::RemoveCircle(int r)
{
	int i,j,t;
	Loopt(VolHeight)
	{
		Loopj(ImHeight)
	      	Loopi(ImWidth)
		{
        if(((i-128)*(i-128)+(j-128)*(j-128))>r*r)
		* get_voxl_color(*this,i,j,t)=0;
		}
	}
}

void CVolumeData::Range(int min,int max)
{
	int i;
	for(i=0;i<ImWidth*ImHeight*VolHeight;i++)
	{
       Buffer[i]=BOUND(Buffer[i],min,max);
	}
}

CVolumeData::~CVolumeData()
{
delete [] this->Buffer;
this->Buffer=0;
}

CVolumeData::CVolumeData()
{
this->Buffer=0;
 this->ImWidth = 0; 
 this->ImHeight= 0;
 this->VolHeight=0; 
 ImgResl[0]=ImgResl[1]=ImgResl[2]=1.0;
}

void CVolumeData::Save(char*FileName)
{
 string imgstr,hdrstr;
 imgstr = FileName; imgstr+=".img";
 hdrstr = FileName; hdrstr+=".hdr";

 FILE*file=fopen((char*)imgstr.c_str(),"wb");
 fwrite((void*)this->Buffer,2,ImWidth*ImHeight*VolHeight,file);
 fclose(file);
 
 file=fopen((char*)hdrstr.c_str(),"wb");
 fprintf(file,"%i %i %i\n",ImWidth,ImHeight,VolHeight);
 fprintf(file,"%lf %lf %lf\n",ImgResl[0],ImgResl[1],ImgResl[2]);
 fprintf(file,"-1024.00 1.00\n");
 fprintf(file,"40\n");
 fprintf(file,"1.000000 0.000000 0.000000 -50.0\n");
 fprintf(file,"0.000000 1.000000 0.000000 -50.0\n");
 fprintf(file,"0.000000 0.000000 1.000000 -50.0\n");
 fprintf(file,"2\n");
 fprintf(file,"0 4095\n");
 fclose(file);

}

void CVolumeData::Load(char*FileName)
{string imgstr,hdrstr,prefixstr;
 prefixstr=FileName;
 imgstr =prefixstr;imgstr +=".img";
 hdrstr =prefixstr;hdrstr +=".hdr";
 double xx, yy, zz;
 read_resolution((char*)prefixstr.c_str(), xx, yy, zz,(double*)ImgResl);
 this->ImWidth = xx; 
 this->ImHeight= yy;
 this->VolHeight=zz;
 this->FormatF(xx,yy,zz);
 FILE*file=fopen(imgstr.c_str(),"rb");
 fread((void*)this->Buffer,2,xx*yy*zz,file);
 fclose(file);
}



void CVolumeData::Clear()
{
int i,j,t;
int cnt;
cnt = ImWidth*ImHeight*VolHeight;
Loopi(cnt)
	Buffer[i]=0;
}

void CVolumeData::FormatF(int w,int h,int sliceNum)
{
delete []Buffer; 
ImWidth =w;
ImHeight=h;
VolHeight=sliceNum;
Buffer=new unsigned short[ImWidth*ImHeight*VolHeight];
ImgResl[0] = ImgResl[1] = ImgResl[2] = 0.5;
}

void CVolumeData::DumpOneSlice(int z)
{
  int slicecnt;
  slicecnt =  ImWidth* ImHeight;
  int i,j;
  C256BitMap Pic;
  Pic.FormatF(ImWidth,ImHeight); 
  int Val;
  for(i=0;i<Pic.Width;i++)
	   for(j=0;j<Pic.Height;j++)
	   {Val= * get_voxl_color(*this,i,j,z);
	    Val= Val * 255 / 4096;
	   * get_pix_color(Pic,i,j)=BOUND(Val,0,255);
	   }
  char sBuff[40];
  sprintf(sBuff,"pic\\dump%03i.bmp",z);
  Pic.Save(sBuff); 
}

#endif
