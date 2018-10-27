/**************************************************
 * TextureTorus.c
 *
 * Program to demonstrate how to attach a texture
 *    map to a torus.  This is the LightTorus program,
 *    modified to display a texture map.
 *
 * The torus can be viewed in wireframe or in "filled
 *     polygon" mode. 
 *
 * The triangles or quadrilaterals are wrapped around the
 * torus in a single long strip.    See figure 13 Chapter I
 * in the book mentioned below.
 *
 *
 * Author: Samuel R. Buss
 *
 * Software accompanying the book
 *		3D Computer Graphics: A Mathematical Introduction with OpenGL,
 *		by S. Buss, Cambridge University Press, 2003.
 *
 * Software is "as-is" and carries no warranty.  It may be used without
 *   restriction, but if you modify it, please change the filenames to
 *   prevent confusion between different versions.
 * Bug reports: Sam Buss, sbuss@ucsd.edu.
 * Web page: http://math.ucsd.edu/~sbuss/MathCG
 *
 * USAGES: There are a number of keyboard commands that control 
 * the animation.  They must be typed into the graphics window, 
 * and are listed below:
 *
 * TURN THE TEXTURE OFF AND ON:
 *   Press "t" to toggle the texture off and on.
 *
 * CONTROLLING RESOLUTION OF THE TORUS MESH
 *   Press "W" to increase the number wraps.
 *   Press "w" to decrease the number wraps.
 *   Press "N" to increase the number of segments per wrap.
 *   Press "n" to decrease the number of segments per wrap.
 *   Press "q" to toggle between quadrangles and triangles.
 *   
 * CONTROLLING THE ANIMATION:
 *   Press the "a" key to toggle the animation off and on.
 *   Press the "s" key to perform a single step of the animation.
 *   The left and right arrow keys controls the
 *		rate of rotation around the y-axis.
 *   The up and down arrow keys increase and decrease the rate of 
 *		rotation around the x-axis.  In order to reverse rotational
 *		direction you must zero or reset the torus ("0" or "r").
 *   Press the "r" key to reset the torus back to initial
 *		position, with no rotation.
 *   Press "0" (zero) to zero the rotation rates.
 *
 * CONTROLLING LIGHTS
 *	 Press '0' or '1' to toggle the first or second light off and on.
 *   Press 'f' to toggle between flat and smooth shading.
 *   Press 'l' to toggle local modes on and off (local viewer and positional light,
 *		or non-local viewer and directional light).
 *
 * COMMANDS SHOWING OPENGL FEATURES:
 *   Pressing "p" toggles between wireframe and polygon mode.
 *   Pressing "f" key toggles between flat and smooth shading.
 *
 **/

#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "RgbImage.h"
#include "TextureTorus.h"
#include <GL/glut.h>				// OpenGL Graphics Utility Library

const float PI2 = 2.0f*3.1415926535;

GLenum runMode = GL_TRUE;

GLenum shadeModel = GL_FLAT;		// Toggles between GL_FLAT and GL_SMOOTH
GLenum polygonMode = GL_LINE;		// Toggles between GL_LINE and GL_FILL

// Variables controlling the animation
float RotX = 0.0f;					// Rotational position around x-axis
float RotY = 0.0f;					// Rotational position around y-axis
float RotIncrementX = 0.0;			// Rotational increment, x-axis
float RotIncrementY = 0.0;			// Rotational increment, y-axis
const float RotIncFactor = 1.5;	// Factor change in rot rate per key stroke

// Variables controlling the fineness of the polygonal mesh
int NumWraps = 10;
int NumPerWrap = 8;

// Variables controlling the size of the torus
float MajorRadius = 3.0;
float MinorRadius = 1.0;

// Mode flags
int QuadMode = 1;				// Quad/Triangle toggling
GLenum LocalMode = GL_TRUE;		// Local viewer/non-local viewer mode
int Light0Flag = 1;				// Is light #0 on?
int Light1Flag = 1;				// Is light #1 on?

// Lighting values
float ambientLight[4] = {0.6, 0.6, 0.6, 1.0};
float Lt0amb[4] = {0.2, 0.2, 0.2, 1.0};
float Lt0diff[4] = {1.0, 1.0, 1.0, 1.0};
float Lt0spec[4] = {1.0, 1.0, 1.0, 1.0};
float Lt0pos[4] = {1.7f*(MajorRadius+MinorRadius), 0.0f, 0.0f, 1.0f};

float Lt1amb[4] = {0.2, 0.2, 0.2, 1.0};
float Lt1diff[4] = {0.7, 0.7, 0.7, 1.0};
float Lt1spec[4] = {1.0, 1.0, 1.0, 1.0};
float Lt1pos[4] = {0.0f, 1.2f*(MajorRadius+MinorRadius), 0.0f, 1.0f};

// Material values
float Noemit[4] = {0.0, 0.0, 0.0, 1.0};
float Matspec[4] = {0.3, 0.3, 0.3, 1.0};
float Matnonspec[4] = {0.4, 0.4, 0.4, 1.0};
float Matshiny = 16.0;

// A texture
int TextureWrapVert=6;
int TextureWrapHoriz=6;
bool textureFlag = true;

// glutKeyboardFunc is called below to set this function to handle
//		all "normal" key presses.
void myKeyboardFunc( unsigned char key, int x, int y ) 
{
	switch ( key ) {
	case 'a':
		runMode = !runMode;
		break;
	case 's':
		runMode = GL_TRUE;
		updateScene();
		runMode = GL_FALSE;
		break;
	case 27:	// Escape key
		exit(1);
	case 'r':	// Reset the animation (resets everything)
		ResetAnimation();
		break;
	case 'z':	// Zero the rotation rates
		ZeroRotation();
		break;
	case 'f':	// Shade mode toggles from flat to smooth
		ShadeModelToggle();
		break;
	case 'p':	// Polygon mode toggles between fill and line
		FillModeToggle();
		break;
	case 'w':	// Decrement number of wraps around torus
		WrapLess();
		break;
	case 'W':	// Increment number of wraps around torus
		WrapMore();
		break;
	case 'n':	// Decrement number of polys per wrap
		NumPerWrapLess();
		break;
	case 'N':	// Increment number of polys per wrap
		NumPerWrapMore();
		break;
	case 'q':	// Toggle between triangles and Quadrilaterals
		QuadTriangleToggle();
		break;
	case 'l':	// Toggle between local and non-local viewer
		LocalToggle();
		break;
	case '0':	// Toggle light #0 on and off
		Light0Toggle();
		break;
	case '1':	// Toggle light #1 on and off
		Light1Toggle();
		break;
	case 't':
		textureFlag = !textureFlag;
		break;
	}
}

// glutSpecialFunc is called below to set this function to handle
//		all "special" key presses.  See glut.h for the names of
//		special keys.
void mySpecialKeyFunc( int key, int x, int y ) 
{
	switch ( key ) {
	case GLUT_KEY_UP:		
		// Either increase upward rotation, or slow downward rotation
		KeyUp();
		break;
	case GLUT_KEY_DOWN:
		// Either increase downwardward rotation, or slow upward rotation
		KeyDown();
		break;
	case GLUT_KEY_LEFT:
		// Either increase left rotation, or slow down rightward rotation.
		KeyLeft();
		break;
	case GLUT_KEY_RIGHT:
		// Either increase right rotation, or slow down leftward rotation.
		KeyRight();
		break;
	}
}

// The routines below are coded so that the only way to change from 
//	one direction of rotation to the opposite direction is to first 
//  reset the animation, 

void KeyUp() {
    if ( RotIncrementX == 0.0 ) {
		RotIncrementX = -0.1;		// Initially, one-tenth degree rotation per update
	}
	else if ( RotIncrementX < 0.0f) {
		RotIncrementX *= RotIncFactor;
	}
	else {
		RotIncrementX /= RotIncFactor;
	}	
}

void KeyDown() {
    if ( RotIncrementX == 0.0 ) {
		RotIncrementX = 0.1;		// Initially, one-tenth degree rotation per update
	}
	else if ( RotIncrementX > 0.0f) {
		RotIncrementX *= RotIncFactor;
	}
	else {
		RotIncrementX /= RotIncFactor;
	}	
}

void KeyLeft() {
    if ( RotIncrementY == 0.0 ) {
		RotIncrementY = -0.1;		// Initially, one-tenth degree rotation per update
	}
	else if ( RotIncrementY < 0.0) {
		RotIncrementY *= RotIncFactor;
	}
	else {
		RotIncrementY /= RotIncFactor;
	}	
}

void KeyRight()
{
    if ( RotIncrementY == 0.0 ) {
		RotIncrementY = 0.1;		// Initially, one-tenth degree rotation per update
	}
	else if ( RotIncrementY > 0.0) {
		RotIncrementY *= RotIncFactor;
	}
	else {
		RotIncrementY /= RotIncFactor;
	}	
}


// Resets position and sets rotation rate back to zero.
void ResetAnimation() {
	RotX = RotY = RotIncrementX = RotIncrementY = 0.0;
}

// Sets rotation rates back to zero.
void ZeroRotation() {
	RotIncrementX = RotIncrementY = 0.0;
}

// Toggle between smooth and flat shading
void ShadeModelToggle() {
	if ( shadeModel == GL_FLAT ) {
		shadeModel = GL_SMOOTH;
	}
	else {
		shadeModel = GL_FLAT;
	}
}

// Toggle between line mode and fill mode for polygons.
void FillModeToggle() {
	if ( polygonMode == GL_LINE ) {
		polygonMode = GL_FILL;
	}
	else {
		polygonMode = GL_LINE;
	}
}

// Toggle quadrilaterial and triangle mode
void QuadTriangleToggle() {
	QuadMode = 1-QuadMode;
}

// Toggle from local to global mode
void LocalToggle() {
	LocalMode = !LocalMode;
	if ( LocalMode ) {
		Lt0pos[3] = Lt1pos[3] = 1.0;	// Put lights back at finite location.
	}
	else {
		Lt0pos[3] = Lt1pos[3] = 0.0;	// Put lights at infinity too.
	}
}

// The next two routines toggle the lights on and off
void Light0Toggle() {
	Light0Flag = 1-Light0Flag;
}

void Light1Toggle() {
	Light1Flag = 1-Light1Flag;
}


// Increment number of wraps
void WrapMore() {
	NumWraps++;
}

// Decrement number of wraps
void WrapLess() {
	if (NumWraps>4) {
		NumWraps--;
	}
}

// Increment number of segments per wrap
void NumPerWrapMore() {
	NumPerWrap++;	
}

// Decrement number segments per wrap
void NumPerWrapLess() {
	if (NumPerWrap>4) {
		NumPerWrap--;
	}
}

/*
 * issue vertex command for segment number j of wrap number i.
 */
void putVertTexture(int i, int j) {
	float wrapFrac = (j%NumPerWrap)/(float)NumPerWrap;
	float wrapFracTex = (float)j/(float)NumPerWrap;
	float phi = PI2*wrapFrac;
	float thetaFrac = ((float)(i%NumWraps)+wrapFracTex)/(float)NumWraps;
	float thetaFracTex = ((float)i+wrapFracTex)/(float)NumWraps;
	float theta = PI2*thetaFrac;
	float sinphi = sin(phi);
	float cosphi = cos(phi);
	float sintheta = sin(theta);
	float costheta = cos(theta);
	float y = MinorRadius*sinphi;
	float r = MajorRadius + MinorRadius*cosphi;
	float x = sintheta*r;
	float z = costheta*r;

	glTexCoord2f( wrapFracTex*(float)TextureWrapVert, thetaFracTex*(float)TextureWrapHoriz );
	glNormal3f(sintheta*cosphi, sinphi, costheta*cosphi);
	glVertex3f(x,y,z);
}


void updateScene( void )
{

	// Clear the redering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glShadeModel( shadeModel );	// Set the shading to flat or smooth.
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);	// Set to be "wire" or "solid"
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, LocalMode); 

	// Set up lights
	glDisable( GL_TEXTURE_2D );
	if ( Light0Flag==1 || Light1Flag==1 ) {
		// Emissive spheres have no other color.
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Noemit);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Noemit);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0);
	}
	if ( Light0Flag==1 ) {
		glPushMatrix();
		glTranslatef(Lt0pos[0], Lt0pos[1], Lt0pos[2]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Lt0diff);
		glutSolidSphere(0.2,5,5);
		glPopMatrix();
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);
	}
	else {
		glDisable(GL_LIGHT0);
	}
	if ( Light1Flag==1 ) {
		glPushMatrix();
		glTranslatef(Lt1pos[0], Lt1pos[1], Lt1pos[2]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Lt1diff);
		glutSolidSphere(0.2,5,5);
		glPopMatrix();
		glEnable(GL_LIGHT1);
		glLightfv(GL_LIGHT1, GL_POSITION, Lt1pos);
	}
	else {
		glDisable(GL_LIGHT1);
	}

	// Torus Materials
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Matnonspec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Matspec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Matshiny);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Noemit);

	if ( textureFlag ) {
		glEnable( GL_TEXTURE_2D );
	}

	glPushMatrix();		// Save to use again next time.

	// Update the orientation of the torus, if the animation is running.
	if ( runMode ) {
		RotY += RotIncrementY;
		if ( fabs(RotY)>360.0 ) {
			RotY -= 360.0*((int)(RotY/360.0));
		}
		RotX += RotIncrementX;
		if ( fabs(RotX)>360.0 ) {
			RotX -= 360.0*((int)(RotX/360.0));
		}
	}
	// Set the orientation.
	glRotatef( RotX, 1.0, 0.0, 0.0);
	glRotatef( RotY, 0.0, 1.0, 0.0);

	// Draw the torus
	for (int i=0; i<NumWraps; i++ ) {
		glBegin( QuadMode ? GL_QUAD_STRIP : GL_TRIANGLE_STRIP);
		for (int j=0; j<=NumPerWrap; j++) {
			putVertTexture(i, j);
			putVertTexture(i+1,j);
		}
		glEnd();
	}

	// Draw a reference sphere
	glDisable( GL_TEXTURE_2D );
	glTranslatef( -MajorRadius-MinorRadius-0.3, 0.0, 0.0);
	glColor3f( 1.0f, 0.0f, 0.0f );
	glutWireSphere( 0.1f, 5, 5 );

	glPopMatrix();		// Restore to original matrix as set in resizeWindow()

	// Flush the pipeline, swap the buffers
    glFlush();
    glutSwapBuffers();
}

/*
 * Read a texture map from a BMP bitmap file.
 */
void loadTextureFromFile(char *filename)
{    
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);

	RgbImage theTexMap( filename );

	// Pixel alignment: each row is word aligned (aligned to a 4 byte boundary)
	//    Therefore, no need to call glPixelStore( GL_UNPACK_ALIGNMENT, ... );

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3,theTexMap.GetNumCols(), theTexMap.GetNumRows(),
					 GL_RGB, GL_UNSIGNED_BYTE, theTexMap.ImageData() );

}

/*
 * Initialize OpenGL: Lights, rendering modes, and textures.
 */
void initRendering()
{
    glEnable( GL_DEPTH_TEST );

	glEnable(GL_LIGHTING);		// Enable lighting calculations
	glEnable(GL_LIGHT0);		// Turn on lights (unnecessary here, since also in Animate()
	glEnable(GL_LIGHT1);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);	// Ambient light

	// Light 0 (Position is set in updateScene)
	glLightfv(GL_LIGHT0, GL_AMBIENT, Lt0amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Lt0spec);

	// Light 1 (Position is set in updateScene)
	glLightfv(GL_LIGHT1, GL_AMBIENT, Lt1amb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, Lt1diff);
	glLightfv(GL_LIGHT1, GL_SPECULAR, Lt1spec);

	// Load the texture
	loadTextureFromFile( "WoodGrain.bmp" );
    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
}

// Called when the window is resized
//		Sets up the projection view matrix (somewhat poorly, however)
void resizeWindow(int w, int h)
{
    float aspectRatio;
	glViewport( 0, 0, w, h );	// View port uses whole window
	h = (w == 0) ? 1 : h;
	aspectRatio = (float)w/(float)h;

	// Set up the proection view matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 60.0, aspectRatio, 1.0, 30.0 );

	// Move system 10 units away to be able to view from the origin.
	// Also tilt the system 15 degrees downward in order to view from above. 
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -10.0);
	glRotatef(15.0, 1.0,0.0,0.0);	 
}


// Main routine
// Set up OpenGL, hook up callbacks, and start the main loop
int main( int argc, char** argv )
{
	glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

	// Window position (from top corner), and size (width and hieght)
    glutInitWindowPosition( 10, 60 );
    glutInitWindowSize( 620, 160 );
    glutCreateWindow( "TextureTorus - Press \"t\" to toggle texture" );

	// Initialize OpenGL rendering modes
    initRendering();
	resizeWindow(620,160);

	// Set up callback functions for key presses
	glutKeyboardFunc( myKeyboardFunc );
	glutSpecialFunc( mySpecialKeyFunc );

	// Set up the callback function for resizing windows
    glutReshapeFunc( resizeWindow );

	// Call this for background processing
	glutIdleFunc( updateScene );
	// Call this whenever window needs redrawing
    glutDisplayFunc( updateScene );
	
	// Start the main loop.  glutMainLoop never returns.
	glutMainLoop(  );

    return(0);	// This line is never reached.
}
