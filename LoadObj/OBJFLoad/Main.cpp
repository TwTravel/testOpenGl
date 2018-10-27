
#include "main.h"										
#include "obj.h"										

bool  g_bFullScreen = true;								
 									


#define FILE_NAME  "foot.obj"							

UINT g_Texture[MAX_TEXTURES] = {0};						

CLoadObj g_Loadobj;									
t3DModel g_3DModel;									

int   g_ViewMode	  = GL_TRIANGLES;
bool  g_bLighting     = true;		
float g_RotateX		  = 0.0f;		
float g_RotationSpeed = 0.8f;		


 
  
