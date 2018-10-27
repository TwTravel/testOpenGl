#include <stdio.h>
#include <stdlib.h>
#include "c24bitmap.h"
#include "c256bitmap.h"
#include "VolRead.h"
#include "cisosurface.h"

int main()
{
	CVolumeData VolDat;
	VolDat.FormatF(256,256,500);

	int i,j,t;
	int x,y,z;

	for(i = 0; i<256; i++)
	{
       for( j = 0; j < 256; j++ )
		   for( t = 0; t < 500; t++ )
		   {
              *get_voxl_color(VolDat,i,j,t) = 0;
		   }
	}

	for(i = 0; i<20; i++)
	{
       for( j= -60; j < 60; j++ )

		   for( t=-60 ; t < 60; t++ )
		   {
			   z = 40  + i;
			   x = 128 + j;
			   y = 128 + t;

              *get_voxl_color(VolDat,x,y,z) = 8000;
		   }
	}

	for(i = 0; i<240; i++)
	{
       for( j= -60; j < -50; j++ )
		   for( t=-60 ; t < 60; t++ )
		   {
			   z = 40  + i;
			   x = 128 + j;
			   y = 128 + t;
              *get_voxl_color(VolDat,x,y,z) = 8000;
			  *get_voxl_color(VolDat,y,x,z) = 8000;
		   }
	}

	for(i = 0; i<240; i++)
	{
       for( j= 50; j < 60; j++ )
		   for( t=-60 ; t < 60; t++ )
		   {
			   z = 40  + i;
			   x = 128 + j;
			   y = 128 + t;

              *get_voxl_color(VolDat,x,y,z) = 8000;
			  *get_voxl_color(VolDat,y,x,z) = 8000;
		   }
	}

	VolDat.Save("ten_test");

	//printf("hello 3d world!\n");
	//=============================================================================
 
	CIsoSurface<unsigned short> isurf;
	double nStepSize = 2.0;	// In millimeters.
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

 file= fopen("shape.obj","wt+");
 
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
