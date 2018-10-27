//////////////////////////////////////////////////////////////////////////////////////////
//	Main.cpp
//	Spherical Harmonic Lighting
//	Downloaded from: www.paulsprojects.net
//	Created:	2nd October 2003
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <vector>
#include "GL files/glee.h"	//header for opengl versions >1.1 and extensions
#include <GL/glu.h>
#include "Maths/Maths.h"
#include "Log/LOG.h"
#include "Timer/TIMER.h"
#include "Fps Counter/FPS_COUNTER.h"
#include "Window/WINDOW.h"
#include "Bitmap Font/BITMAP_FONT.h"
#include "Image/IMAGE.h"
#include "Samples/SAMPLE.h"
#include "Samples/GenerateSamples.h"
#include "Light/Light.h"
#include "Bounding Volumes/Bounding Volumes.h"
#include "Ray/RAY.h"
#include "SH Scene/SH_SCENE.h"
#include "SH Rotation/SH Rotation.h"
#include "Main.h"

//link to libraries
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "winmm.lib")

TIMER timer;
FPS_COUNTER fpsCounter;
BITMAP_FONT font;

COLOR backgroundColor(0.0f, 0.0f, 0.0f, 0.0f);

//Samples on surface of sphere
const int sqrtNumSamples=50;
const int numSamples=sqrtNumSamples*sqrtNumSamples;
SAMPLE * samples=NULL;

//Number of SH bands and functions
const int numBands=4;
const int numFunctions=numBands*numBands;

//SH coefficients for light source, before and after rotation
double lightCoeffs[numFunctions];
double rotatedLightCoeffs[numFunctions];

//Scene to light
SH_SCENE shScene;

//Type of lighting to use
enum LIGHTING_TYPE
{
	LIGHTING_TYPE_GL,
	LIGHTING_TYPE_SH_UNSHADOWED,
	LIGHTING_TYPE_SH_SHADOWED
};

LIGHTING_TYPE lightingType=LIGHTING_TYPE_SH_SHADOWED;

//Polar angles for light rotation
double theta=43.3, phi=225.0;

//Set up GL
bool GLInit()
{
	//Init window
	if(!WINDOW::Instance()->Init(	"Spherical Harmonic Lighting", 640, 480, 8, 8, 8, 8, 24, 8,
									false, false, true))
		return false;
	
	//Check for OpenGL version/extensions
	//if(!extgl_Extensions.OpenGL14)
	//{
		//LOG::Instance()->OutputError("I require OpenGL 1.4 support");
		//return false;
	//}

	//Init font
	if(!font.Init())
		return false;

	//set viewport
	int height=WINDOW::Instance()->height;
	if(height==0)
		height=1;

	glViewport(0, 0, WINDOW::Instance()->width, height);

	//Set up projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(	45.0f, (GLfloat)WINDOW::Instance()->width/(GLfloat)WINDOW::Instance()->height,
					1.0f, 100.0f);

	//Load identity modelview
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Shading states
	glShadeModel(GL_SMOOTH);
	glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//Depth states
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);

	return true;
}

//Set up variables
bool DemoInit()
{
	//Create space for the samples
	samples=new SAMPLE[numSamples];
	if(!samples)
	{
		LOG::Instance()->OutputError("Unable to allocate space for samples");
		return false;
	}

	//Generate the samples
	if(!GenerateSamples(sqrtNumSamples, numBands, samples))
		return false;

	//Generate the light coefficients by projecting the Light function onto the SH functions
	for(int i=0; i<numFunctions; ++i)
	{
		lightCoeffs[i]=0.0;

		for(int j=0; j<numSamples; ++j)
			lightCoeffs[i]+=Light(samples[j].theta, samples[j].phi)*samples[j].shValues[i];

		lightCoeffs[i]*=4*M_PI/numSamples;
	}

	//Set up the rotated light coefficients
	RotateSHCoefficients(numBands, lightCoeffs, rotatedLightCoeffs, theta, phi);

	//Generate the scene
	if(!shScene.Generate())
		return false;
	
	//Generate the coefficients for the direct lighting transfer functions
	if(!shScene.GenerateDirectCoeffs(numBands, numSamples, samples))
		return false;

	//reset timer
	timer.Reset();

	return true;
}

//Perform per-frame updates
void UpdateFrame()
{
	//set currentTime and timePassed
	static double lastTime=timer.GetTime();
	double currentTime=timer.GetTime();
	double timePassed=currentTime-lastTime;
	lastTime=currentTime;

	//Update window
	WINDOW::Instance()->Update();

	//Change lighting mode
	if(WINDOW::Instance()->IsKeyPressed('1'))
		lightingType=LIGHTING_TYPE_GL;

	if(WINDOW::Instance()->IsKeyPressed('2'))
		lightingType=LIGHTING_TYPE_SH_UNSHADOWED;

	if(WINDOW::Instance()->IsKeyPressed('3'))
		lightingType=LIGHTING_TYPE_SH_SHADOWED;

	//Update the light angles if the mouse button is pressed
	if(WINDOW::Instance()->IsLeftButtonPressed())
	{
		theta+=0.1*WINDOW::Instance()->GetMouseYMovement();

		//Clamp theta to [0, 180]
		if(theta<0.0)
			theta=0.0;

		if(theta>180.0)
			theta=180.0;

		phi+=0.1*WINDOW::Instance()->GetMouseXMovement();
		
		//Update the rotated light coefficients
		RotateSHCoefficients(numBands, lightCoeffs, rotatedLightCoeffs, theta, phi);
	}

	//Calculate the lit color of each vertex
	int numObjects=shScene.objects.size();

	for(int i=0; i<numObjects; ++i)
	{
		SH_OBJECT * currentObject=shScene.objects[i];
		int numVertices=currentObject->vertices.size();

		if(lightingType==LIGHTING_TYPE_SH_UNSHADOWED)
		{
			for(int j=0; j<numVertices; ++j)
			{
				SH_VERTEX & currentVertex=currentObject->vertices[j];

				double brightness=0.0;

				for(int k=0; k<numFunctions; ++k)
					brightness+=rotatedLightCoeffs[k]*currentVertex.unshadowedCoeffs[k];
				
				currentVertex.litColor=(float)brightness*currentVertex.diffuseMaterial;
			}
		}

		if(lightingType==LIGHTING_TYPE_SH_SHADOWED)
		{
			for(int j=0; j<numVertices; ++j)
			{
				SH_VERTEX & currentVertex=currentObject->vertices[j];

				double brightness=0.0;

				for(int k=0; k<numFunctions; ++k)
					brightness+=rotatedLightCoeffs[k]*currentVertex.shadowedCoeffs[k];
				
				currentVertex.litColor=(float)brightness*currentVertex.diffuseMaterial;
			}
		}
	}
	
	//Render frame
	RenderFrame(currentTime, timePassed);
}

//Render a frame
void RenderFrame(double currentTime, double timePassed)
{
	//Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();										//reset modelview matrix
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	//Use positive Z axis upwards coordinate system
	gluLookAt(	-4.0f, 6.0f, 4.0f,
				 0.0f, 0.0f, 0.0f,
				 0.0f, 1.0f, 0.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

	//Enable vertex arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	//Set up the GL light if necessary
	if(lightingType==LIGHTING_TYPE_GL)
	{
		//Calculate the light direction
		VECTOR4D lightDir(0.0f, 0.0f, 1.0f, 0.0f);
		lightDir.RotateY(theta);
		lightDir.RotateZ(phi);
		
		glLightfv(GL_LIGHT0, GL_POSITION, lightDir);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, white*0.6f);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);

		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);

		//Enable normal array
		glEnableClientState(GL_NORMAL_ARRAY);
	}		

	//Loop through and draw the objects in the scene
	for(std::size_t i=0; i<shScene.objects.size(); ++i)
	{
		SH_OBJECT * currentObject=shScene.objects[i];

		glVertexPointer(3, GL_FLOAT, sizeof(SH_VERTEX), &currentObject->vertices[0].position);
		glNormalPointer(GL_FLOAT, sizeof(SH_VERTEX), &currentObject->vertices[0].normal);
		
		if(lightingType==LIGHTING_TYPE_GL)
			glColorPointer(	3, GL_FLOAT, sizeof(SH_VERTEX),
							&currentObject->vertices[0].diffuseMaterial);
		else
			glColorPointer(	3, GL_FLOAT, sizeof(SH_VERTEX),
							&currentObject->vertices[0].litColor);
		
		glDrawElements(	GL_TRIANGLES, currentObject->indices.size(), GL_UNSIGNED_INT,
						&currentObject->indices[0]);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	if(lightingType==LIGHTING_TYPE_GL)
	{
		glDisable(GL_LIGHTING);
		glDisableClientState(GL_NORMAL_ARRAY);
	}


	fpsCounter.Update();

	//Print text
	font.StartTextMode();

	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	font.Print(0, 28, "FPS: %.2f", fpsCounter.GetFps());
	
	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	if(lightingType==LIGHTING_TYPE_GL)
		font.Print(0, 48, "GL Lighting");
	if(lightingType==LIGHTING_TYPE_SH_UNSHADOWED)
		font.Print(0, 48, "SH Lighting: Unshadowed");
	if(lightingType==LIGHTING_TYPE_SH_SHADOWED)
		font.Print(0, 48, "SH Lighting: Shadowed");

	font.EndTextMode();

	WINDOW::Instance()->SwapBuffers();

	//Save a screenshot
	if(WINDOW::Instance()->IsKeyPressed(VK_F1))
	{
		WINDOW::Instance()->SaveScreenshot();
		WINDOW::Instance()->SetKeyReleased(VK_F1);
	}

	//Check for an openGL error
	WINDOW::Instance()->CheckGLError();

	//quit if necessary
	if(WINDOW::Instance()->IsKeyPressed(VK_ESCAPE))
		PostQuitMessage(0);
}

//Shut down demo
void DemoShutdown()
{
	font.Shutdown();
	WINDOW::Instance()->Shutdown();
}

//WinMain
int WINAPI WinMain(	HINSTANCE	hInstance,			//Instance
					HINSTANCE	hPrevInstance,		//Previous Instance
					LPSTR		lpCmdLine,			//Command line params
					int			nShowCmd)			//Window show state
{
	//Save hInstance
	WINDOW::Instance()->hInstance=hInstance;

	//Init GL and variables
	if(!GLInit())
	{
		LOG::Instance()->OutputError("OpenGL Initiation Failed");
		return false;
	}
	else
		LOG::Instance()->OutputSuccess("OpenGL Initiation Successful");

	if(!DemoInit())
	{
		LOG::Instance()->OutputError("Demo Initiation Failed");
		return false;
	}
	else
		LOG::Instance()->OutputSuccess("Demo Initiation Successful");

	//Main Loop
	for(;;)
	{
		if(!(WINDOW::Instance()->HandleMessages()))	//quit if HandleMessages returns false
			break;

		UpdateFrame();
	}

	//Shutdown
	DemoShutdown();

	//Exit program
	LOG::Instance()->OutputSuccess("Exiting...");
	return 0;
}
