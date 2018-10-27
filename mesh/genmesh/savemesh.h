#ifndef savemesh_h
#define savemesh_h
#include "VolRead.h"
#include "cisosurface.h"
#include ".\mesh\meshutil.h"
#include <string>
using namespace std;

/*struct triangle_index
{
int idx_[3];
};*/

/*triangle_index
{
int idx_[3];
double vox_inten;*/
//<------------------------
int MeshSmoothing1( double *pMeshPoint, char *pPointType, int nMeshPoint, int *pTriangle, 
				  int nTriangle, int nMaxIter );

 struct PVMeshIdx
{
 int LAtst,LAted;
 int PV1st,PV1ed;
 int PV2st,PV2ed;
 int PV3st,PV3ed;
 int PV4st,PV4ed;
}; 

void GetTrianglVox(CVolumeData&Vol,
				   vector<POINT3DID>&points_,
				   triangle_index&Tri)
{
//CVolumeData 

POINT3DID Point;

 int i,j,t,tri_idx;

 for(tri_idx=0;tri_idx<3;tri_idx++)
 {
Point.x = (points_[Tri.idx_[tri_idx]].x);//+points_[Tri.idx_[1]].x+points_[Tri.idx_[2]].x)/3.0;
Point.y = (points_[Tri.idx_[tri_idx]].y);//+points_[Tri.idx_[1]].y+points_[Tri.idx_[2]].y)/3.0;
Point.z = (points_[Tri.idx_[tri_idx]].z);//+points_[Tri.idx_[1]].z+points_[Tri.idx_[2]].z)/3.0;

	for(i=0;i<4;i++)
		  {	 
	   Tri.vox_inten = * get_voxl_color(Vol, Point.x-i,Point.y,Point.z);
		if(Tri.vox_inten >0 ) return;
	   Tri.vox_inten = * get_voxl_color(Vol, Point.x+i,Point.y,Point.z);
		if(Tri.vox_inten >0 ) return; 
//----------------------------------------------------
	   Tri.vox_inten = * get_voxl_color(Vol, Point.x,Point.y-i,Point.z);
		if(Tri.vox_inten >0 ) return;
	   Tri.vox_inten = * get_voxl_color(Vol, Point.x,Point.y+i,Point.z);
		if(Tri.vox_inten >0 ) return; 
//----------------------------------------------------
	   Tri.vox_inten = * get_voxl_color(Vol, Point.x,Point.y,Point.z-i);
		if(Tri.vox_inten >0 ) return;
	   Tri.vox_inten = * get_voxl_color(Vol, Point.x,Point.y,Point.z+i);
		if(Tri.vox_inten >0 ) return; 
//----------------------------------------------------
	   Tri.vox_inten = * get_voxl_color(Vol, Point.x-i,Point.y-i,Point.z-i);
		if(Tri.vox_inten >0 ) return;
	   Tri.vox_inten = * get_voxl_color(Vol, Point.x+i,Point.y+i,Point.z+i);
		if(Tri.vox_inten >0 ) return; 
		  }

 }
 
 printf("%f,%f,%f,----failed\n",
	 points_[Tri.idx_[0]].x,
	 points_[Tri.idx_[0]].y,
	 points_[Tri.idx_[0]].z);
}

void ConvertVolumeToPt( CVolumeData&Vol,
					    string fnameprefix,
						vector<POINT3DID>&points_,vector<triangle_index>&itriangles_,
						int&nTriangles,int&nPoints,PVMeshIdx&MshIdx)
{
	int i;

	CIsoSurface<unsigned short> isurf;
	double nStepSize = 2.0;	// In millimeters.
	int xStepSize = max( 1, int( nStepSize/Vol.ImgResl[0]+0.5 ) );
	int yStepSize = max( 1, int( nStepSize/Vol.ImgResl[1]+0.5 ) );
	int zStepSize = max( 1, int( nStepSize/Vol.ImgResl[2]+0.5 ) );

isurf.GenerateSurface( Vol.Buffer, 50,Vol.ImWidth,Vol.ImHeight,Vol.VolHeight, 
(float)Vol.ImgResl[0], (float)Vol.ImgResl[1], (float)Vol.ImgResl[2],
	xStepSize, yStepSize, zStepSize );

 nPoints    = isurf.m_nVertices;
 nTriangles	= isurf.m_nTriangles;
 
//MeshSmoothing( double *pMeshPoint, char *pPointType, int nMeshPoint, int *pTriangle, 
//				  int nTriangle, int nMaxIter );
//-------------------------------------------------------------------------
string    fn_tbin,fn_pbin;
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

MeshSmoothing1( smooth_buf,  0,   nPoints, isurf.m_piTriangleIndices, 
 			   nTriangles, 15);

printf("triangles:%i\n", nTriangles);
printf("meshes:   %i\n", nPoints);
points_.resize(nPoints);
 
	for (i=0; i<nPoints; i++)
	{	 
		VData[i]           = smooth_buf[3*i+0];
		VData[i+  nPoints] = smooth_buf[3*i+1];
		VData[i+2*nPoints] = smooth_buf[3*i+2];

		points_[i].x = smooth_buf[3*i+0]/Vol.ImgResl[0];
		points_[i].y = smooth_buf[3*i+1]/Vol.ImgResl[1];
		points_[i].z = smooth_buf[3*i+2]/Vol.ImgResl[2];

	}
    
	file=fopen((char*)fn_pbin.c_str(),"wb+");
    fwrite(VData, sizeof(double),nPoints*3, file);
    fclose(file);
//-------------------------------------------------
    itriangles_.resize(nTriangles);

	  for (i=0; i<nTriangles; i++)
	{
		 
		itriangles_[i].idx_[0] = isurf.m_piTriangleIndices[i*3+0];
		itriangles_[i].idx_[1] = isurf.m_piTriangleIndices[i*3+1];
		itriangles_[i].idx_[2] = isurf.m_piTriangleIndices[i*3+2];

	    GetTrianglVox(Vol,points_,itriangles_[i]);
	}

	  
	  sort(itriangles_.begin(),itriangles_.end());

	  printf("//------------//\\------------\n");

	  //

	   for(i=0; i<nTriangles-1; i++)
	 {		 
		 if(itriangles_[i].vox_inten != itriangles_[i+1].vox_inten)
		 {
			 printf("%i,%lf\n",i,itriangles_[i].vox_inten);

     switch(int(itriangles_[i].vox_inten))
	 {
	  case 1500:
		  { MshIdx.LAtst = 0;
		    MshIdx.LAted = i;
		  	break; }
 
	  case 2010:
		  { MshIdx.PV1st = MshIdx.LAted+1;
		    MshIdx.PV1ed = i;
		  	break; }

	  case 2020:
		  { MshIdx.PV2st = MshIdx.PV1ed+1;
		  	MshIdx.PV2ed = i;
		  	break; }

	  case 2030:
		  { MshIdx.PV3st = MshIdx.PV2ed+1;
		  	MshIdx.PV3ed = i;
            MshIdx.PV4st = i+1;
		  	MshIdx.PV4ed = nTriangles-1;
		  	break; }

	  default:
		  break;} 
		 }
	  }

	 printf("//------------//\\------------\n");
//-------------------------------------------------    
	    for(i=0; i<nTriangles; i++)
	  { 
		IdxBuffer[i]              = itriangles_[i].idx_[0] ;
		IdxBuffer[i+  nTriangles] = itriangles_[i].idx_[1] ;
		IdxBuffer[i+2*nTriangles] = itriangles_[i].idx_[2] ;
	  }
   
	file = fopen((char*)fn_tbin.c_str(),"wb+");

	fwrite(IdxBuffer, sizeof(double), 3*nTriangles, file); 
	fclose(file);
	delete []VData;
	delete []IdxBuffer;
	delete []smooth_buf;

}
 

 //----------------------------------------------------------
 //////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\file		MeshUtil.cpp
\brief		Mesh smoothing.
\note		It is based on the following paper.
			Gabriel Taubin, "Curve and Surface Smoothing Without Shrinkage," ICCV, pp. 852-857, 1995.
\author		Yefeng Zheng
\author		Copyright (C) Siemens Corporate Research 2006 
\author		All Rights Reserved
\date		Sept. 2006
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////

#include <set>
#include <map>
#include ".\mesh\MeshUtil.h"
//#include "ShapeSpace.h"
//#include "MemoryLeakDetection.h"

//////////////////////////////////////////////////////////////////////////////////////
/*!
\brief			Add a point to the neigborhood of another point. No redundent neighbors.
\param[in,out]	MeshPoint		Neighborhood of a point.
\param[in]		NeighborIndex	A new neighboring point to add.
\return			0 for success and others for failure.
*/
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
/*!
\brief			Calculate the neighborhood for all mesh points.
\param[in]		pTriangle	Mesh triangles.
\param[in]		nTriangle	Number of mesh triangles.
\param[in,out]	Neighbor	Neighborhood information extracted. The memory of Neighbor
							should be pre-allocated.
\return			0 for success and others for failure.
*/
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
/*!
\brief			Mesh smoothing. The major work is done in this function.
\param[in,out]	pMeshPoint		Mesh points.
\param[in]		Neighbor		Neighborhood information.
\param[in]		nMaxIter		Maximum interation for smoothing.
\return			0 for success and others for failure.
*/
//////////////////////////////////////////////////////////////////////////////////////
int MeshSmoothing( double *pMeshPoint, vector<NEIGHBOR> &Neighbor, int nMaxIter=10 )
{
	int		i, j;
	double	lambda	= 0.330;
	double	mu		= -0.3315;

	// Set up the working array.
	vector<tIds3dPoint>	NewPnt(Neighbor.size());

	// Mesh smoothing.
	for( int nIter=0; nIter<2*nMaxIter; nIter++ )
	{
		double weight;
		if( nIter%2 == 0 )
			weight = lambda;
		else
			weight = mu;

		for( i=0; i<(int)Neighbor.size(); i++ )
		{
			int nID = Neighbor[i].nID;
			tIds3dPoint CurPnt = tIds3dPoint( pMeshPoint[3*nID], pMeshPoint[3*nID+1], pMeshPoint[3*nID+2] );
			tIds3dPoint	Delta = tIds3dPoint(0, 0, 0);
			if( Neighbor[i].nNeighbor > 0 )
			{
				for( j=0; j<Neighbor[i].nNeighbor; j++ )
				{
					int nIndex	= Neighbor[i].NeighborList[j];
					tIds3dPoint NgbPnt	= tIds3dPoint( pMeshPoint[3*nIndex], pMeshPoint[3*nIndex+1], pMeshPoint[3*nIndex+2] );
					Delta += NgbPnt - CurPnt;
				}
				Delta /= Neighbor[i].nNeighbor;
			}
			NewPnt[i] = CurPnt + weight*Delta;
		}
		// Set up the smoothed point set.
		for( i=0; i<(int)Neighbor.size(); i++ )
		{ 
			int nID = Neighbor[i].nID;
			pMeshPoint[3*nID]	= NewPnt[i][0];
			pMeshPoint[3*nID+1] = NewPnt[i][1];
			pMeshPoint[3*nID+2] = NewPnt[i][2];
		}
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////
/*!
\brief			Mesh smoothing for a set of points.
\param[in,out]	pMeshPoint		Mesh points.
\param[in]		pPointType		Point type, 0 for normal points and 1 for control points.
								Control points are not updated.
\param[in]		nMeshPoint		Number of mesh points.
\param[in]		pTriangle		Mesh triangles.
\param[in]		nTriangle		Number of mesh triangles.
\param[in]		nMaxIter		Maximum interation for smoothing.
\return			0 for success and others for failure.
*/
//////////////////////////////////////////////////////////////////////////////////////
int MeshSmoothing1( double *pMeshPoint, char *pPointType, int nMeshPoint, int *pTriangle, 
				  int nTriangle, int nMaxIter )
{
	// Generate the neighborhood.
	vector<NEIGHBOR> Neighbor;
	Neighbor.resize( nMeshPoint );
	if( GetNeighborhood( pTriangle, nTriangle, Neighbor ) != 0 )
		return -1;

	// Do not update the control points.
	for( int i=0; i<nMeshPoint; i++ )
	{
		if( pPointType == NULL )
			continue;
		if( pPointType[i] == 0 )
			continue;
		Neighbor[i].nNeighbor  = 0;
	}

	// Mesh smoothing.
	MeshSmoothing( pMeshPoint, Neighbor, nMaxIter );

	return 0;		
}

//////////////////////////////////////////////////////////////////////////////////////
/*!
\brief			Smooth a curve (open or closed). For an open curve, the first and last
				points are fixed. While, for a closed curve, all points will be smoothed.
\param[in,out]	pPoint		A set of points to be smoothed.
\param[in]		nPoint		Number of points
\param[in]		bClosed		Closed or not.
\param[in]		nMaxIter	Maximum iteration for smoothing.
\return			0 for success and others for failure.
*/
//////////////////////////////////////////////////////////////////////////////////////
int CurveSmoothing( double *pPoint, int nPoint, bool bClosed, int nMaxIter )
{
	int i;
	if( pPoint == NULL || nPoint <= 0 )
		return -1;

	// Set up the working array.
	vector<tIds3dPoint>	NewPnt(nPoint);

	// Mesh smoothing.
	double	weight  = 0.25;
	for( int nIter=0; nIter<nMaxIter; nIter++ )
	{
		int nStart, nEnd;
		if( bClosed )
		{ // A closed curve.
			nStart	= 0;
			nEnd	= nPoint-1;
		}
		else
		{ // An open curve;
			nStart  = 1;
			nEnd	= nPoint-2;
		}
		for( i=nStart; i<=nEnd; i++ )
		{
			int j = (i-1+nPoint)%nPoint;
			int k = (i+1)%nPoint;
			tIds3dPoint	C  = tIds3dPoint( pPoint[3*i], pPoint[3*i+1], pPoint[3*i+2] );
			tIds3dPoint	N1 = tIds3dPoint( pPoint[3*j], pPoint[3*j+1], pPoint[3*j+2] );
			tIds3dPoint N2 = tIds3dPoint( pPoint[3*k], pPoint[3*k+1], pPoint[3*k+2] );
			tIds3dPoint	Delta = 0.5*( (N1-C) + (N2-C) );
			NewPnt[i] = C + weight*Delta;
		}
		// Set up the smoothed point set.
		for( i=nStart; i<=nEnd; i++ )
		{
			pPoint[3*i]		= NewPnt[i][0];
			pPoint[3*i+1]	= NewPnt[i][1];
			pPoint[3*i+2]	= NewPnt[i][2];
		}
	}

	return 0;
}

/*[ Code Review -----------------------------------------------------------------------------------------
   Review method         		: 4-Eye-Check 
   Review date             		: 6-August-2008
   Review members      			: Yefeng Zheng & Xiaoguang Lu, SCR-DS
   Review result            	: accepted
   Review coverage       		: all 
   Hazard code modified since
   previous inspection? 		: n.a. 
   Label reviewed	     		: SCR_CARDIACCT_HEARTCHAMBER_BASELINE_20080701
]-------------------------------------------------------------------------------------------------------*/


#endif