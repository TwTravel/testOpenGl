#ifndef Get_triangle_H
#define Get_triangle_H

#include <stdio.h>
#include <stdlib.h>
#include "cisosurface.h"
#include "VolRead.h"
#include "savemesh.h"

void GenerateTriangle(CVolumeData&Vol,char*fn_mesh,PVMeshIdx&MshIdx)
{
 int triangle_num,mesh_num;
string fn_save,fn_tbin,fn_pbin;
vector<POINT3DID>       points_;
vector<triangle_index>  itriangles_;
 

 ConvertVolumeToPt( Vol, fn_mesh,points_, itriangles_,triangle_num,mesh_num,MshIdx); 

string meshfilename;
meshfilename = fn_mesh;
meshfilename +=".mesh";
/*256 256 206
0.890587 0.890587 0.890587
1
0
17568 8836
dst.tbin
dst.pbin
1
0
dst.obin*/
FILE*file=fopen(meshfilename.c_str(),"wt+");
fprintf(file,"%i %i %i\n",Vol.ImWidth,Vol.ImHeight,Vol.VolHeight); 
fprintf(file,"%lf %lf %lf\n",Vol.ImgResl[0],Vol.ImgResl[1],Vol.ImgResl[2]); 
fprintf(file,"1\n0\n");
fprintf(file,"%i %i\n",triangle_num,mesh_num);
fprintf(file,"%s.tbin\n%s.pbin\n",fn_mesh,fn_mesh);
fprintf(file,"1\n0\n");
fprintf(file,"%s.obin",fn_mesh);
fclose(file);
}

#endif