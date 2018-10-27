**************************************************************************
**	Spherical Harmonic Lighting
**
**	www.paulsprojects.net
**
**	paul@paulsprojects.net
**************************************************************************

Description:

This program displays a simple looking scene (actually containing approximately 25,000 vertices) which can be lit in 3 different ways. As well as standard OpenGL lighting, the scene can be lit by two techniques which make use of spherical harmonics. This method produces global illumination style images in real time, but usually only for static objects.

The first time this demo is executed, it will take approximately 30 minutes (AMD Athlon XP 2000) to generate the spherical harmonic coefficients at each vertex. These are then saved to a file so they can be retrieved quickly in future.

For information on the theory behind this project, see "technical.txt". 


Requirements:

No extensions required


References:

"Spherical Harmonic Lighting, the Gritty Details", Robin Green.
http://www.research.scea.com/gdc2003/spherical-harmonic-lighting.html


Keys:

F1		-	Take a screenshot
Escape		-	Quit

Hold left mouse button & move mouse: Move light source

1		-	Standard GL lighting
2		-	SH lighting, unshadowed
3		-	SH lighting, shadowed
