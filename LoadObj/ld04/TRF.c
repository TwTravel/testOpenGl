/*
    transformation.c
    Nate Robins, 1997

    Tool for teaching about OpenGL transformations.
 
*/


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <string>
#include <GL/glut.h>
#include "glm.h"


#include "c24bitmap.h"
#include "c256bitmap.h"


double g_RotateX =0;
double g_RotateY =0;
double g_RotateZ =0;


void saveFrameBuff( int width, int height)
{   
    C24BitMap CPic;  
	
	
    BYTE * pPixelData;
    
    int i, j;
    int PixelDataLength;

     
    glReadBuffer(GL_FRONT);

    // 计算像素数据的实际长度
    // 默认是3通道
    i = width * 3;      // 得到每一行的像素数据长度
    while (i % 4 != 0)  // 补充数据，直到 i 是的倍数
        ++i;

    PixelDataLength = i * height;

    // 但这里仅追求直观，对速度没有太高要求PixelDataLength = i * WindowHeight;
    // 分配内存和打开文件
    pPixelData = (BYTE *)malloc(PixelDataLength);
    if (pPixelData == 0)
        exit(0);



    // 读取像素
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glReadPixels(0, 0, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, pPixelData);
     
	 char fileName[200];
     sprintf(fileName, "savepic%06.2lfx%06.2lfy%06.2lfz.bmp", g_RotateX, g_RotateY, g_RotateZ);
	 
     CPic.FormatF( width,  height );
	 memcpy(CPic.Buffer, pPixelData, PixelDataLength);
	 
     CPic.Save(fileName);
	 
     free(pPixelData);
}
 
//###########################################################################
//###########################################################################
//###########################################################################
using namespace std;

#pragma comment( linker, "/entry:\"mainCRTStartup\"" )  // set the entry point to be main()


typedef struct _cell {
    int id;
    int x, y;
    float min, max;
    float value;
    float step;
    char* info;
    char* format;
} cell;


cell translation[3] = {
    { 1, 120, 40, -5.0, 5.0, 0.0, 0.01,
        "Specifies X coordinate of translation vector.", "%.2f" },
    { 2, 180, 40, -5.0, 5.0, 0.0, 0.01,
    "Specifies Y coordinate of translation vector.", "%.2f" },
    { 3, 240, 40, -5.0, 5.0, 0.0, 0.01,
    "Specifies Z coordinate of translation vector.", "%.2f" },
};

cell rotation[4] = {
    { 4, 120, 80, -360.0, 360.0, 0.0, 1.0,
        "Specifies angle of rotation, in degrees.", "%.1f" },
    { 5, 180, 80, -1.0, 1.0, 0.0, 0.01,
    "Specifies X coordinate of vector to rotate about.", "%.2f" },
    { 6, 240, 80, -1.0, 1.0, 1.0, 0.01,
    "Specifies Y coordinate of vector to rotate about.", "%.2f" },
    { 7, 300, 80, -1.0, 1.0, 0.0, 0.01,
    "Specifies Z coordinate of vector to rotate about.", "%.2f" },
};

cell scale[3] = {
    {  8, 120, 120, -5.0, 5.0, 1.0, 0.01,
        "Specifies scale factor along X axis.", "%.2f" },
    {  9, 180, 120, -5.0, 5.0, 1.0, 0.01,
    "Specifies scale factor along Y axis.", "%.2f" },
    { 10, 240, 120, -5.0, 5.0, 1.0, 0.01,
    "Specifies scale factor along Z axis.", "%.2f" },
};

GLfloat eye[3] = { 0.0, 0.0, 2.0 };
GLfloat at[3]  = { 0.0, 0.0, 0.0 };
GLfloat up[3]  = { 0.0, 1.0, 0.0 };

GLboolean swapped = GL_FALSE;
GLboolean world_draw = GL_TRUE;
GLMmodel* pmodel = NULL;
GLint selection = 0;

void redisplay_all(void);
GLdouble projection[16], modelview[16], inverse[16];
GLuint window,   screen ;
GLuint sub_width = 256, sub_height = 256;


GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_10;

void
setfont(char* name, int size)
{
    font_style = GLUT_BITMAP_HELVETICA_10;
    if (strcmp(name, "helvetica") == 0) {
        if (size == 12) 
            font_style = GLUT_BITMAP_HELVETICA_12;
        else if (size == 18)
            font_style = GLUT_BITMAP_HELVETICA_18;
    } else if (strcmp(name, "times roman") == 0) {
        font_style = GLUT_BITMAP_TIMES_ROMAN_10;
        if (size == 24)
            font_style = GLUT_BITMAP_TIMES_ROMAN_24;
    } else if (strcmp(name, "8x13") == 0) {
        font_style = GLUT_BITMAP_8_BY_13;
    } else if (strcmp(name, "9x15") == 0) {
        font_style = GLUT_BITMAP_9_BY_15;
    }
}

void 
drawstr(GLuint x, GLuint y, char* format, ...)
{
    va_list args;
    char buffer[255], *s;
    
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    
    glRasterPos2i(x, y);
    for (s = buffer; *s; s++)
        glutBitmapCharacter(font_style, *s);
}



 
void drawmodel(char* filename)
{
    if (!pmodel) {
        pmodel = glmReadOBJ(filename);
        if (!pmodel) exit(0);
        glmUnitize(pmodel);
        glmFacetNormals(pmodel);
        glmVertexNormals(pmodel, 90.0);
    }
    
    glmDraw(pmodel, GLM_SMOOTH | GLM_MATERIAL);
}

 


void main_reshape(int width,  int height) 
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, height, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
#define GAP  25             /* gap between subwindows */
    sub_width = (width-GAP*2);
    sub_height = (height-GAP*2);
    
    glutSetWindow(screen);
    glutPositionWindow(GAP, GAP);
    glutReshapeWindow(sub_width, sub_height);
     
    //glutPositionWindow(GAP, GAP+sub_height+GAP);
    //glutReshapeWindow(sub_width+GAP+sub_width, sub_height);
}

void
main_display(void)
{
    glClearColor(0.8, 0.8, 0.8, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3ub(0, 0, 0);
    setfont("helvetica", 12);
    drawstr(GAP, GAP-5, "World-space view");
    drawstr(GAP+sub_width+GAP, GAP-5, "Screen-space view");
    drawstr(GAP, GAP+sub_height+GAP-5, "Command manipulation window");
    glutSwapBuffers();
}

void
main_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 's':
        swapped = !swapped;
        break;
	case 'a':		
		g_RotateX += 2;
		printf("RotateX: %f\n",g_RotateX);
		break;
	case 'd':
		g_RotateX -= 2;
		printf("RotateX: %f\n",g_RotateX);
		break;
		
	case 'w':		
		g_RotateY += 2;
		printf("RotateX: %f\n",g_RotateX);
		break;
	case 'x':
		g_RotateY -= 2;
		printf("RotateX: %f\n",g_RotateX);
		break;

	case '1':		
		g_RotateZ += 2;
		printf("RotateX: %f\n",g_RotateX);
		break;
	case '2':
		g_RotateZ -= 2;
		printf("RotateX: %f\n",g_RotateX);
		break;
		
    case 'r':
        translation[0].value = 0.0;
        translation[1].value = 0.0;
        translation[2].value = 0.0;
        rotation[0].value = 0.0;
        rotation[1].value = 0.0;
        rotation[2].value = 0.0;
        rotation[3].value = 0.0;
        scale[0].value = 1.0;
        scale[1].value = 1.0;
        scale[2].value = 1.0;
        break;
    case 27:
        exit(0);
    }
    
    redisplay_all();
}



 

void
screen_reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)width/height, 0.5, 8.0);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye[0], eye[1], eye[2], at[0], at[1], at[2], up[0], up[1],up[2]);
	
	glRotatef(g_RotateX, 0, 1.0f, 0);
	glRotatef(g_RotateY, 1.0f, 0, 0);
	glRotatef(g_RotateZ, 0, 0, 1.0f);
	glScalef(0.5, 0.5, 0.5);
    /*if (swapped) {
        glRotatef(rotation[0].value, rotation[1].value, 
            rotation[2].value, rotation[3].value);
        glTranslatef(translation[0].value, translation[1].value,
            translation[2].value);
    } else {
        glTranslatef(translation[0].value, translation[1].value,
            translation[2].value);
        glRotatef(rotation[0].value, rotation[1].value, 
            rotation[2].value, rotation[3].value);
    }*/
    glScalef(scale[0].value, scale[1].value, scale[2].value);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glClearColor(0.2, 0.2, 0.2, 0.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
	
	saveFrameBuff(  width,  height);
}

string  filenamestr;

/*
cell material_Ka[4] = {
    { 25, 220, 260, 0.0, 1.0, 0.2, 0.01,
        "Specifies ambient red reflectance of the material.", "%.2f" },
    { 26, 280, 260, 0.0, 1.0, 0.2, 0.01,
    "Specifies ambient green reflectance of the material.", "%.2f" },
    { 27, 340, 260, 0.0, 1.0, 0.2, 0.01,
    "Specifies ambient blue reflectance of the material.", "%.2f" },
    { 28, 400, 260, 0.0, 1.0, 1.0, 0.01,
    "Specifies ambient alpha reflectance of the material.", "%.2f" },
};

cell material_Kd[4] = {
    { 29, 220, 290, 0.0, 1.0, 0.8, 0.01,
        "Specifies diffuse red reflectance of the material.", "%.2f" },
    { 30, 280, 290, 0.0, 1.0, 0.8, 0.01,
    "Specifies diffuse green reflectance of the material.", "%.2f" },
    { 31, 340, 290, 0.0, 1.0, 0.8, 0.01,
    "Specifies diffuse blue reflectance of the material.", "%.2f" },
    { 32, 400, 290, 0.0, 1.0, 1.0, 0.01,
    "Specifies diffuse alpha reflectance of the material.", "%.2f" },
};

cell material_Ks[4] = {
    { 33, 220, 320, 0.0, 1.0, 1.0, 0.01,
        "Specifies specular red reflectance of the material.", "%.2f" },
    { 34, 280, 320, 0.0, 1.0, 1.0, 0.01,
    "Specifies specular green reflectance of the material.", "%.2f" },
    { 35, 340, 320, 0.0, 1.0, 1.0, 0.01,
    "Specifies specular blue reflectance of the material.", "%.2f" },
    { 36, 400, 320, 0.0, 1.0, 1.0, 0.01,
    "Specifies specular alpha reflectance of the material.", "%.2f" },
};

cell material_Ke[4] = {
    { 37, 220, 350, 0.0, 1.0, 0.0, 0.01,
     "Specifies red emitted light intensity of the material.", "%.2f" },
    { 38, 280, 350, 0.0, 1.0, 0.0, 0.01,
    "Specifies green emitted light intensity of the material.", "%.2f" },
    { 39, 340, 350, 0.0, 1.0, 0.0, 0.01,
    "Specifies blue emitted light intensity of the material.", "%.2f" },
    { 40, 400, 350, 0.0, 1.0, 1.0, 0.01,
    "Specifies alpha emitted light intensity of the material.", "%.2f" },
};

cell material_Se = { 41, 200, 380, 0.0, 128.0, 50.0, 1.0,
"Specifies the specular exponent of the material.", "%.0f" };
*/

float mKa[] ={0.6,0.0,0.1,0.5};
float mKd[] ={0.6,0.0,0.1,0.32};
float mKs[] ={0.6,0.0,0.1,0.125};
float mKe[] ={0.6,0.1,0.1,0.0};
float material_Se_value = 15;

void
screen_display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, mKa);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mKd);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mKs);
    glMaterialfv(GL_FRONT, GL_EMISSION, mKe);
    glMaterialf(GL_FRONT, GL_SHININESS, material_Se_value);
    drawmodel((char*)filenamestr.c_str());
    glutSwapBuffers();
}

void
screen_menu(int value)
{
    char* name = 0;
    
    switch (value) {
    case 'a':
        name = "data/al.obj";
        break;
    case 's':
        name = "data/soccerball.obj";
        break;
    case 'd':
        name = "data/dolphins.obj";
        break;
    case 'f':
        name = "data/flowers.obj";
        break;
    case 'j':
        name = "data/f-16.obj";
        break;
    case 'p':
        name = "data/porsche.obj";
        break;
    case 'r':
        name = "data/rose+vase.obj";
        break;
    }
    
    if (name) {
        pmodel = glmReadOBJ(name);
        if (!pmodel) exit(0);
        glmUnitize(pmodel);
        glmFacetNormals(pmodel);
        glmVertexNormals(pmodel, 90.0);
    }
    
    redisplay_all();
}

 

int old_y;

 
void
redisplay_all(void)
{
   //glutSetWindow(command);
    //glutPostRedisplay();
   // glutSetWindow(world);
    //glutPostRedisplay();
	
    glutSetWindow(screen);
    screen_reshape(sub_width, sub_height);
    glutPostRedisplay();
}

int
main(int argc, char** argv)
{
	filenamestr = argv[1];
	
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(512+GAP*3, 512+GAP*3);
    glutInitWindowPosition(50, 50);
    glutInit(&argc, argv);
    
    window = glutCreateWindow("Transformation");
    glutReshapeFunc(main_reshape);
    glutDisplayFunc(main_display);
    glutKeyboardFunc(main_keyboard);

 
    screen = glutCreateSubWindow(window, GAP+256+GAP, GAP, 256, 256);
    glutReshapeFunc(screen_reshape);
    glutDisplayFunc(screen_display);
    glutKeyboardFunc(main_keyboard);
    glutCreateMenu(screen_menu);
    glutAddMenuEntry("Models", 0);
    glutAddMenuEntry("", 0);
    glutAddMenuEntry("Soccerball", 's');
    glutAddMenuEntry("Al Capone", 'a');
    glutAddMenuEntry("F-16 Jet", 'j');
    glutAddMenuEntry("Dolphins", 'd');
    glutAddMenuEntry("Flowers", 'f');
    glutAddMenuEntry("Porsche", 'p');
    glutAddMenuEntry("Rose", 'r');
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutMainLoop();
    
    return 0;
}
