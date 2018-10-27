 
#include <math.h>		// For math routines (such as sqrt & trig).
#include <stdio.h>
#include <stdlib.h>		// For the "exit" function
#include <GL/glut.h>	// OpenGL Graphics Utility Library


//###########################################################################################
//###########################################################################################
//###########################################################################################


//#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <vector>
#include <GL/gl.h>										
#include <GL/glu.h>										
//#include <GL/glaux.h>
//#include <crtdbg.h>
using namespace std;

#define SCREEN_WIDTH 400								
#define SCREEN_HEIGHT 300								
#define SCREEN_DEPTH 16									


#define MAX_TEXTURES 100								// 最大的纹理数目

typedef unsigned char BYTE;

// 定义3D点的类，用于保存模型中的顶点
class CVector3 
{
public:
	float x, y, z;
};

// 定义2D点类，用于保存模型的UV纹理坐标
class CVector2 
{
public:
	float x, y;
};

//  面的结构定义
struct tFace
{
	int vertIndex[3];			// 顶点索引
	int coordIndex[3];			// 纹理坐标索引
};

//  材质信息结构体
struct tMaterialInfo
{
	char  strName[255];			// 纹理名称
	char  strFile[255];			// 如果存在纹理映射，则表示纹理文件名称
	BYTE  color[3];				// 对象的RGB颜色
	int   texureId;				// 纹理ID
	float uTile;				// u 重复
	float vTile;				// v 重复
	float uOffset;			    // u 纹理偏移
	float vOffset;				// v 纹理偏移
} ;

//  对象信息结构体
struct t3DObject 
{
	int  numOfVerts;			// 模型中顶点的数目
	int  numOfFaces;			// 模型中面的数目
	int  numTexVertex;			// 模型中纹理坐标的数目
	int  materialID;			// 纹理ID
	bool bHasTexture;			// 是否具有纹理映射
	char strName[255];			// 对象的名称
	CVector3  *pVerts;			// 对象的顶点
	CVector3  *pNormals;		// 对象的法向量
	CVector2  *pTexVerts;		// 纹理UV坐标
	tFace *pFaces;				// 对象的面信息
};

//  模型信息结构体
struct t3DModel 
{
	int numOfObjects;					// 模型中对象的数目
	int numOfMaterials;					// 模型中材质的数目
	vector<tMaterialInfo> pMaterials;	// 材质链表信息
	vector<t3DObject> pObject;			// 模型中对象链表信息
};

#include "Obj.h"
//###########################################################################################
bool  g_bFullScreen = true;
#define FILE_NAME  "foot.obj"
unsigned int g_Texture[MAX_TEXTURES] = {0};						

CLoadObj g_Loadobj;									
t3DModel g_3DModel;									

int   g_ViewMode	  = GL_TRIANGLES;
bool  g_bLighting     = true;		
float g_RotateX		  = 0.0f;		
float g_RotationSpeed = 0.8f;	

//###########################################################################################
//###########################################################################################
//###########################################################################################

void RenderScene() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glLoadIdentity();									

	gluLookAt(		0, 1.5f, 8,		0, 0.5f, 0,			0, 1, 0);
	
	glRotatef(g_RotateX, 0, 1.0f, 0);
	g_RotateX += g_RotationSpeed;	

	// 遍历模型中所有的对象
	for(int i = 0; i < g_3DModel.numOfObjects; i++)
	{
		// 如果对象的大小小于0，则退出
		if(g_3DModel.pObject.size() <= 0) break;

		// 获得当前显示的对象
		t3DObject *pObject = &g_3DModel.pObject[i];
			
		// 判断该对象是否有纹理映射
		if(pObject->bHasTexture) {

			// 打开纹理映射
			glEnable(GL_TEXTURE_2D);
			glColor3ub(255, 255, 255);
			glBindTexture(GL_TEXTURE_2D, g_Texture[pObject->materialID]);
		} else {

			// 关闭纹理映射
			glDisable(GL_TEXTURE_2D);
			glColor3ub(255, 255, 255);
		}
		// 开始以g_ViewMode模式绘制
		glBegin(g_ViewMode);					
			// 遍历所有的面
			for(int j = 0; j < pObject->numOfFaces; j++)
			{
				// 遍历三角形的所有点
				for(int whichVertex = 0; whichVertex < 3; whichVertex++)
				{
					// 获得面对每个点的索引
					int index = pObject->pFaces[j].vertIndex[whichVertex];
			
					// 给出法向量
					glNormal3f(pObject->pNormals[ index ].x, pObject->pNormals[ index ].y, pObject->pNormals[ index ].z);
				
					// 如果对象具有纹理
					if(pObject->bHasTexture) {

						// 确定是否有UVW纹理坐标
						if(pObject->pTexVerts) {
							glTexCoord2f(pObject->pTexVerts[ index ].x, pObject->pTexVerts[ index ].y);
						}
					} else {

						if(g_3DModel.pMaterials.size() && pObject->materialID >= 0) 
						{
							BYTE *pColor = g_3DModel.pMaterials[pObject->materialID].color;
							glColor3ub(pColor[0], pColor[1], pColor[2]);
						}
					}
					glVertex3f(pObject->pVerts[ index ].x, pObject->pVerts[ index ].y, pObject->pVerts[ index ].z);
				}
			}

		glEnd();								// 绘制结束
	}

	glutSwapBuffers( );									// 交换缓冲区
}


void SizeOpenGLScreen(int width, int height)		
{
	if (height==0)										
	{
		height=1;										
	}

	glViewport(0,0,width,height);						

	glMatrixMode(GL_PROJECTION);		
	glLoadIdentity();						
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height, .5f ,150.0f);

	glMatrixMode(GL_MODELVIEW);						
	glLoadIdentity();									
}

void InitializeOpenGL(int width, int height) 
{  
     		

	glEnable(GL_TEXTURE_2D);				
	glEnable(GL_DEPTH_TEST);				

	SizeOpenGLScreen(width, height);	
}

/*void Init( )
{
	 							
}*/

//###########################################################################################
//###########################################################################################
//###########################################################################################

void myKeyboardFunc( unsigned char key, int x, int y );
void mySpecialKeyFunc( int key, int x, int y );
//void drawScene(void);
void initRendering();
void resizeWindow(int w, int h);


int RunMode = 1;		// Used as a boolean (1 or 0) for "on" and "off"

// The next global variable controls the animation's state and speed.
float CurrentAngle = 0.0f;			// Angle in degrees
float AnimateStep = 3.0f;			// Rotation step per update

// These variables set the dimensions of the rectanglar region we wish to view.
const double Xmin = 0.0, Xmax = 3.0;
const double Ymin = 0.0, Ymax = 3.0;

// glutKeyboardFunc is called below to set this function to handle
//		all "normal" key presses.
void myKeyboardFunc( unsigned char key, int x, int y )
{
	switch ( key ) {
	case 'r':
		RunMode = 1-RunMode;		// Toggle to opposite value
		if ( RunMode==1 ) {
			glutPostRedisplay();
		}
		break;
	case 's':
		RunMode = 1;
		//drawScene();
		RunMode = 0;
		break;
	case 27:	// Escape key
		exit(1);
	}
}

// glutSpecialFunc is called below to set this function to handle
//		all "special" key presses.  See glut.h for the names of
//		special keys.
void mySpecialKeyFunc( int key, int x, int y )
{
	switch ( key ) {
	case GLUT_KEY_UP:		
		if ( AnimateStep < 1.0e3) {			// Avoid overflow problems
			AnimateStep *= sqrt(2.0);		// Increase the angle increment
		}
		break;
	case GLUT_KEY_DOWN:
		if (AnimateStep>1.0e-6) {		// Avoid underflow problems.
			AnimateStep /= sqrt(2.0);	// Decrease the angle increment
		}
		break;
	}
}



 

// Initialize OpenGL's rendering modes
void initRendering()
{
    glShadeModel( GL_FLAT );	// The default value of GL_SMOOTH is usually better
    glEnable( GL_DEPTH_TEST );	// Depth testing must be turned on
	
	//####################################################################################
	//GetClientRect(g_hWnd, &g_rRect);					
	//InitializeOpenGL(g_rRect.right, g_rRect.bottom);

	g_Loadobj.ImportObj(&g_3DModel, FILE_NAME);			// 将obj文件装入到模型结构体中

	// 遍历所有的材质
	/*for(int i = 0; i < g_3DModel.numOfMaterials; i++)
	{
		// 判断是否是一个文件名
		if(strlen(g_3DModel.pMaterials[i].strFile) > 0)
		{
			//  使用纹理文件名称来装入位图
			CreateTexture(g_Texture, g_3DModel.pMaterials[i].strFile, i);			
		}

		// 设置材质的纹理ID
		g_3DModel.pMaterials[i].texureId = i;
	}*/

	glEnable(GL_LIGHT0);								
	glEnable(GL_LIGHTING);								
	glEnable(GL_COLOR_MATERIAL);					
}

// Called when the window is resized
//		w, h - width and height of the window in pixels.
void resizeWindow(int w, int h)
{
	double scale, center;
	double windowXmin, windowXmax, windowYmin, windowYmax;

	// Define the portion of the window used for OpenGL rendering.
	glViewport( 0, 0, w, h );	// View port uses whole window

	// Set up the projection view matrix: orthographic projection
	// Determine the min and max values for x and y that should appear in the window.
	// The complication is that the aspect ratio of the window may not match the
	//		aspect ratio of the scene we want to view.
	w = (w==0) ? 1 : w;
	h = (h==0) ? 1 : h;
	if ( (Xmax-Xmin)/w < (Ymax-Ymin)/h ) {
		scale = ((Ymax-Ymin)/h)/((Xmax-Xmin)/w);
		center = (Xmax+Xmin)/2;
		windowXmin = center - (center-Xmin)*scale;
		windowXmax = center + (Xmax-center)*scale;
		windowYmin = Ymin;
		windowYmax = Ymax;
	}
	else {
		scale = ((Xmax-Xmin)/w)/((Ymax-Ymin)/h);
		center = (Ymax+Ymin)/2;
		windowYmin = center - (center-Ymin)*scale;
		windowYmax = center + (Ymax-center)*scale;
		windowXmin = Xmin;
		windowXmax = Xmax;
	}
	
	// Now that we know the max & min values for x & y 
	//		that should be visible in the window,
	//		we set up the orthographic projection.
    /*glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( windowXmin, windowXmax, windowYmin, windowYmax, -1, 1 );*/
	int width,height;
	width  = w;
	height = h;
	//glViewport(0,0,width,height);						

	glMatrixMode(GL_PROJECTION);		
	glLoadIdentity();						
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height, .5f ,150.0f);

	glMatrixMode(GL_MODELVIEW);						
	glLoadIdentity();

}


// Main routine
// Set up OpenGL, define the callbacks and start the main loop
int main( int argc, char** argv )
{
	glutInit(&argc,argv);
    
	// We're going to animate it, so double buffer 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    
	initRendering();
	
	// Window position (from top corner), and size (width% and hieght)
    glutInitWindowPosition( 10, 60 );
    glutInitWindowSize( 800, 600 );
    glutCreateWindow( "SimpleAnim" );

	// Initialize OpenGL as we like it..
    

	// Set up callback functions for key presses
	glutKeyboardFunc( myKeyboardFunc );			// Handles "normal" ascii symbols
	glutSpecialFunc( mySpecialKeyFunc );		// Handles "special" keyboard keys

	// Set up the callback function for resizing windows
    glutReshapeFunc( resizeWindow );

	// Call this for background processing
	// glutIdleFunc( myIdleFunction );

	// call this whenever window needs redrawing
    glutDisplayFunc( RenderScene );

	fprintf(stdout, "Arrow keys control speed.  Press \"r\" to run,  \"s\" to single step.\n");
	
	// Start the main loop.  glutMainLoop never returns.
	glutMainLoop(  );

    return(0);	// This line is never reached.
}
