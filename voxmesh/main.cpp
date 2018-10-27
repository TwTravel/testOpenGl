#include <stdio.h>
#include <stdlib.h>
#include "c24bitmap.h"
#include "c256bitmap.h"
#include "VolRead.h"
#include "cisosurface.h"

void swapval(int &a,int&b)
{
	int temp = a;
	a = b;
	b = temp;
}

int main(int argc, char* argv[])
{
	C24BitMap CPicMask[5];
	C256BitMap GPic;
	//CPicMask.Load(argv[1]);
	
	CPicMask[0].Load("0.bmp");
	CPicMask[1].Load("1.bmp");
	CPicMask[2].Load("2.bmp");
	CPicMask[3].Load("3.bmp");
	CPicMask[4].Load("4.bmp");
	
	CVolumeData VolDat;
	VolDat.FormatF(500,500,500);
    GPic.FormatF(500,500);
	int i,j,t;
	int x,y,z;

	for(i = 0; i<500; i++)
	{
       for( j = 0; j < 500; j++ )
	   {  
           *get_pix_color(GPic,i,j) =0;
            for( t = 0; t < 500; t++ )
		   {
              *get_voxl_color(VolDat,i,j,t) = 0;
		   }
	   }
	}
	
	for(i = 0; i < 412; i++)
	{
       for( j = 0; j < 412; j++ )
	   {  
          int x,y,z;
		  //#######################0############################
          C24PixVal Pix = get_pix_color(CPicMask[0],i,j);
		   
		  if(!( (*Pix.g)>200 && (*Pix.r)<50 && (*Pix.b)<50))
		  {
			 *get_pix_color(GPic,i+44,j+44) =255;
			 for( t = 0; t < 30; t++ )
		   {
			 x = i + 44;  y = j + 44; z = t + 44;
			 z = 500 -1 - z;
              *get_voxl_color(VolDat,x,y,z) = 5000;
		   }
		  }
		   //#######################1############################
          Pix = get_pix_color(CPicMask[1],i,j);
		   
		  if(!( (*Pix.g)>200 && (*Pix.r)<50 && (*Pix.b)<50))
		  {
			 *get_pix_color(GPic,i+44,j+44) =255;
			 for( t = 0; t < 30; t++ )
		   {
			 x = i + 44;  y = j + 44; z = t + 44;
			 z = 500 -1 - z;
			 swapval(y , z);
              *get_voxl_color(VolDat,x,y,z) = 5000;
		   }
		  }
		   //######################2#############################
          Pix = get_pix_color(CPicMask[2],i,j);
		   
		  if(!( (*Pix.g)>200 && (*Pix.r)<50 && (*Pix.b)<50))
		  {
			 *get_pix_color(GPic,i+44,j+44) =255;
			 for( t = 0; t < 30; t++ )
		   {
			 x = i + 44;  y = j + 44; z = t + 44;
			 swapval(y, z);
             *get_voxl_color(VolDat,x,y,z) = 5000;
		   }
		  }
		   //######################3#############################
          Pix = get_pix_color(CPicMask[3],i,j);
		   
		  if(!( (*Pix.g)>200 && (*Pix.r)<50 && (*Pix.b)<50))
		  {
			 *get_pix_color(GPic,i+44,j+44) =255;
			 for( t = 0; t < 30; t++ )
		   {
			 x = i + 44;  y = j + 44; z = t + 44;
			 z = 500 -1 - z;
			 swapval(x,z);
              *get_voxl_color(VolDat,x,y,z) = 5000;
		   }
		  }	
		   //########################4###########################
          Pix = get_pix_color(CPicMask[4],i,j);
		   
		  if(!( (*Pix.g)>200 && (*Pix.r)<50 && (*Pix.b)<50))
		  {
			 *get_pix_color(GPic,i+44,j+44) =255;
			 for( t = 0; t < 30; t++ )
		   {
			 x = i + 44;  y = j + 44; z = t + 44;
			 swapval(x, z);
              *get_voxl_color(VolDat,x,y,z) = 5000;
		   }
		  }		  
	   }
	}
	
	GPic.Save("gray.bmp");
	 /*
	VolDat.Save("ten_test");*/

	//printf("hello 3d world!\n");
	//=============================================================================
 
	CIsoSurface<unsigned short> isurf;
	double nStepSize = 1.0;	// In millimeters.
	
	VolDat.ImgResl[0] = VolDat.ImgResl[1] = VolDat.ImgResl[2] =0.5;
	
	int xStepSize = max( 1, int( nStepSize/VolDat.ImgResl[0]+0.5 ) );
	int yStepSize = max( 1, int( nStepSize/VolDat.ImgResl[1]+0.5 ) );
	int zStepSize = max( 1, int( nStepSize/VolDat.ImgResl[2]+0.5 ) );

   isurf.GenerateSurface( VolDat.Buffer, 50, VolDat.ImWidth ,VolDat.ImHeight, VolDat.VolHeight, 
   (float)VolDat.ImgResl[0], (float)VolDat.ImgResl[1], (float)VolDat.ImgResl[2],
	xStepSize, yStepSize, zStepSize );

   int nPoints      = isurf.m_nVertices;
   int nTriangles	= isurf.m_nTriangles;
 
//MeshSmoothing( double *pMeshPoint, char *pPointType, int nMeshPoint, int *pTriangle, 
//				  int nTriangle, int nMaxIter );
//-------------------------------------------------------------------------
string    fn_tbin,fn_pbin,fnameprefix;
fnameprefix ="demoshape";
fn_tbin = fnameprefix+".tbin";
fn_pbin = fnameprefix+".pbin";
FILE* file;
double* VData    = new double[nPoints*3];
double*smooth_buf= new double[nPoints*3];
double*IdxBuffer = new double[nTriangles*3]; 
 
	for (i=0; i<nPoints; i++)
	{
		smooth_buf[3*i+0]	= isurf.m_ppt3dVertices[i][0];
		smooth_buf[3*i+1]	= isurf.m_ppt3dVertices[i][1];
		smooth_buf[3*i+2]	= isurf.m_ppt3dVertices[i][2];
	}

 //file= fopen(argv[2],"wt+");
 
file= fopen("hollowcube.obj","wt+");

 //int cout;

 for (i=0; i<nPoints; i++)
{
	fprintf(file, "v %6.2f  %6.2f  %6.2f\n", isurf.m_ppt3dVertices[i][0], isurf.m_ppt3dVertices[i][1], isurf.m_ppt3dVertices[i][2]);
}

  for (i=0; i<nTriangles; i++)
{
	fprintf(file, "f %i  %i  %i\n", isurf.m_piTriangleIndices[i*3+0]+1, isurf.m_piTriangleIndices[i*3+1]+1, isurf.m_piTriangleIndices[i*3+2]+1);
}

 fclose(file);
	return 1;
}
