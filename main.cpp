/**
 * <p>Title: Assignment # 4 main file</p>
 * <p>Description: CS 337 Assignment # 4</p>
 * <p>Copyright: Copyright © 2005</p>
 * <p>Company: UW Ñ River Falls</p>
 * @author Kyle Thompson
 * @email kyle.thompson@uwrf.edu
 * @date November 11th, 2005
 * @team 
 * @version 1.0
 */

#pragma mark includes
#include <iostream> // used for: cout
#include <math.h> // used for: sin(), cos()
#include <time.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include "util.h"
#include "file.h"



#pragma mark defines
#define kViewAreaMax 480.0f // distance of the far side of the perspective viewing volume from the camera
#define kViewAreaMin 16.0f // distance of the near side of the perspective viewing volume from the camera

#define kCameraCoasting true // whether the camera should coast from a rotate or not



#pragma mark globals
int gMainWindow; // the main GLUT window
double gSpeedMultiplier = 1.0f; // the speed of the simulation; halved or doubled with the [-] and [+] keys
bool gSpeedPause = false; // the pausing of the simulation; paused or unpaused with the [  ] or [P] key

GLubyte *gTexData;
GLuint gTex;

GLfloat gCubePos[3] = {0.0f, 0.0f, 0.0f}; // position of the cube
GLfloat gPyramidPos[3] = {0.0f, 0.0f, 0.0f}; // position of the pyramid
GLfloat gCubeVel[3] = {15.0f, 10.0f, -50.0f}; // velocity of the cube
GLfloat gPyramidVel[3] = {30.0f, 30.0f, 75.0f}; // velocity of the pyramid
GLfloat gCubeSize = 75.0f; // size of the cube
GLfloat gPyramidSize = 100.0f; // size of the pyramid
GLfloat gCubeRot[3] = {0.0f, 0.0f, 0.0f}; // rotation of the cube
GLfloat gPyramidRot[3] = {0.0f, 0.0f, 0.0f}; // rotation of the pyramid
GLfloat gCubeRotDel[3] = {5.0f, 7.5f, 15.0f}; // rotation speed of the cube
GLfloat gPyramidRotDel[3] = {0.0f, -135.0f, 0.0f}; // rotation speed of the pyramid

GLfloat gBoundBox[6] = {-30.0f, 30.0f, -60.0f, 60.0f, -60.0f, 60.0f}; // coordinates of the bounding box the objects stay within

GLfloat gLight0Pos[] = {50.0f, 300.0f, 50.0f, 1.0f}; // light position
GLfloat gLight1Pos[] = {-150.0f, -300.0f, 0.0f, 1.0f}; // light position
GLfloat gLight2Pos[] = {-200.0f, 100.0f, -200.0f, 1.0f}; // light position

int gMouseX; // stores the last mouse down x coordinate
int gMouseY; // stores the last mouse down y coordinate
bool gIsRotating = false; // keeps track of whether we are rotating right now

GLfloat gCameraPos[3] = {0.0f, 0.0f, 0.0f}; // position of the camera; derived from its roll (Y angle) and altitude
GLfloat gCameraYRot = M_PI / 2.0f; // camera's roll/rotation amount in radians around center
GLfloat gCameraXRot = M_PI / 2.0f; // camera's pitch/rotation amount in radians around center
GLfloat gCameraYRotVel = 0.0f; // velocity of the camera's roll/rotation around the center
GLfloat gCameraXRotVel = 0.0f; // velocity of the camera's pitch/rotation around the center
GLfloat gCameraRotDivider = 2000.0f / (2.0f * M_PI); // the amount to divide the mouse movement by to get the rotation
GLfloat gCameraAltitude = 180.0f; // current altitude of the camera above the center

int gWinWidth, gWinHeight;

bool gIsFullscreen = false;
//bool gWaitForKey = false;



#pragma mark prototypes
int main(int argc, char * argv[]);
void init(void);
void initTextures(void);
void dealloc(void);
void setupLighting(void);
void resize(int width, int height);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void keyboard(unsigned char key, int x, int y);
void update(int value);
void display(void);
void drawAxes(void);
void draw3DLayers(void);
void draw3DCubes(void);
void drawCube(GLubyte *color, int widthI, int heightI, int depthI);
//void drawTexPyramid(GLfloat base, GLfloat height);
//void drawTexCube(GLfloat size);
bounceSides bounceCheck(GLfloat objPos[3], GLfloat objVel[3], GLfloat objSize);
bounceSides bounceCheck(GLfloat objPos[3], GLfloat objVel[3], GLfloat objSize[3]);
void drawDepthBuffer(void);



#pragma mark functions

/*!
    @function   main()
    @abstract   main function used for intialization, setting up, and starting GLUT
*/
int main(int argc, char *argv[])
{
	/*std::cout << "Texture Mapping: Assignment # 4\n\n";
	
	std::cout << "Press the [-] and [+] keys to change the speed of the simulation.\n";
	std::cout << "Press the [  ] or [P] key to pause the simulation.\n";
	std::cout << "Press the [R] key to reverse the simulation speed.\n";
	std::cout << "Click and drag to rotate the camera.\n\n";*/
	
	// set up GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB /*| GLUT_DOUBLE*/ | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 300);
	
	gMainWindow = glutCreateWindow("Texture Mapping: Assignment # 4");
	glutReshapeFunc(resize); // window resize callback
	glutDisplayFunc(display); // window display callback
	glutMouseFunc(mouse); // mouse down/up callback for scene rotation
	glutMotionFunc(motion); // motion callback used for mouse movement tracking for scene rotation
	glutKeyboardFunc(keyboard); // key press callback; used for changing the speed of the simulation
	//glutIdleFunc(update); // idle callback; used for updating the scene and telling OpenGL to redraw
	glutTimerFunc(0, update, 1); // timer callback; used for updating the scene and telling OpenGL to redraw
	init();
	setupLighting();
	
	// main program loop
	glutMainLoop();
	
	dealloc();
	
    return 0;
}



/*!
    @function   init()
    @abstract   sets up some OpenGL settings
*/
void init(void)
{
	glShadeModel(GL_SMOOTH);
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_COLOR_MATERIAL);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	
	//glEnable(GL_LIGHTING);
	
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	initTextures();
}



/*!
    @function   initTextures()
    @abstract   sets up some OpenGL settings
*/
void initTextures(void)
{
	gTexData = initRGBAImageDataFromFile("brownstone.rgba", 60, 120, 120);
	if (gTexData == NULL)
		exit(0); // couldn't find the texture map
	glGenTextures(1, &gTex);
	glBindTexture(GL_TEXTURE_3D, gTex);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage3D(GL_TEXTURE_3D, 0, 4, 60, 120, 120, 0, GL_RGBA, GL_UNSIGNED_BYTE, gTexData);
	//delete [] gTexData; // disable when freeing this in dealloc()
}



/*!
    @function   dealloc()
    @abstract   frees up allocated memory; not called in some implementations of GLUT
*/
void dealloc(void)
{
	//delete [] gTexData; // disable when freeing this in init()
}



/*!
    @function   setupLighting()
    @abstract   sets up OpenGL lighting for the scene
*/
void setupLighting(void)
{
	// enable lighting and a light source
	glEnable(GL_LIGHT0);
	
	// light attributes
	GLfloat ambient0[] = {0.0f, 0.0f, 0.0f, 1.0f}; // ambient light intensity
	GLfloat diffuse0[] = {0.5f, 0.5f, 0.5f, 1.0f}; // diffuse light intensity
	GLfloat specular0[] = {1.0f, 1.0f, 1.0f, 1.0f}; // specular light intensity
	
	// set the attributes
	glLightfv(GL_LIGHT0, GL_POSITION, gLight0Pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);
	
	// enable a 2nd light source
	glEnable(GL_LIGHT1);
	
	// light attributes
	GLfloat ambient1[] = {0.0f, 0.0f, 0.0f, 1.0f}; // ambient light intensity
	GLfloat diffuse1[] = {0.1f, 0.1f, 0.1f, 1.0f}; // diffuse light intensity
	GLfloat specular1[] = {0.0f, 0.0f, 0.0f, 1.0f}; // specular light intensity
	
	// set the attributes
	glLightfv(GL_LIGHT1, GL_POSITION, gLight1Pos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);
	
	// enable a 3rd light source
	glEnable(GL_LIGHT2);
	
	// light attributes
	GLfloat ambient2[] = {0.0f, 0.0f, 0.0f, 1.0f}; // ambient light intensity
	GLfloat diffuse2[] = {0.5f, 0.5f, 0.5f, 1.0f}; // diffuse light intensity
	GLfloat specular2[] = {0.0f, 0.0f, 0.0f, 1.0f}; // specular light intensity
	
	// set the attributes
	glLightfv(GL_LIGHT2, GL_POSITION, gLight2Pos);
	glLightfv(GL_LIGHT2, GL_AMBIENT, ambient2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, specular2);
}



/*!
    @function   resize()
    @abstract   GLUT window resize callback function; sets glViewport and glOrtho depending on how the window was resized
    @param      int width: the newly resized width
    @param      int height: the newly resized height
*/
void resize(int width, int height)
{
	if (height == 0)  height = 1;
	int h = gWinHeight = (int)height;
	int w = gWinWidth = (int)width;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// calculates the same -200 to 200 X and Y box to be in the middle of the window with space added
	// to the sides or top and bottom to keep it in the center for different proportions
	/*if (w <= h) {
		gBoundBox[0] = -200.0f;
		gBoundBox[1] = 200.0f;
		gBoundBox[2] = -200.0f * h / w;
		gBoundBox[3] = 200.0f * h / w;
		gBoundBox[4] = 200.0f;
		gBoundBox[5] = -200.0f;
	} else {
		gBoundBox[0] = -200.0f * w / h;
		gBoundBox[1] = 200.0f * w / h;
		gBoundBox[2] = -200.0f;
		gBoundBox[3] = 200.0f;
		gBoundBox[4] = 200.0f;
		gBoundBox[5] = -200.0f;
	}
	
	glOrtho(gBoundBox[0], gBoundBox[1], gBoundBox[2], gBoundBox[3], gBoundBox[4], gBoundBox[5]);*/
	
	// set view to perspective view
	gluPerspective(60.0f, (GLfloat)w / h, kViewAreaMin, kViewAreaMax);
	
	update(0); // run an update after resizing
}


/*!
    @function   mouse()
    @abstract   GLUT mouse event callback function; keeps track of when a button is pressed
    @param      int button: the button that was pressed
    @param      int state: what the button did (most usefully, if it was pressed)
    @param      int x: the mouse's x coordinate when the button was pressed
    @param      int y: the mouse's y coordinate when the button was pressed
*/
void mouse(int button, int state, int x, int y)
{
	switch (button) // (allows expansion for multiple buttons)
	{
		case GLUT_LEFT_BUTTON: // check for left mouse button
			if (state == GLUT_DOWN) { // when the mouse button is down,
				gMouseX = x; // update the mouse origin x position
				gMouseY = y; // update the mouse origin x position
				gIsRotating = true; // mark that we are now rotating
			} else { // when the mouse button goes up
				gIsRotating = false; // mark that we are no longer rotating
			}
			break;
	}
}



/*!
    @function   motion()
    @abstract   GLUT mouse event callback function; changes scene properties as a pressed mouse is moved
    @param      int x: the mouse's new x coordinate
    @param      int y: the mouse's new y coordinate
*/
void motion(int x, int y)
{
	if (gIsRotating) { // is being rotated by the user
		gCameraYRotVel = -(x - gMouseX) / gCameraRotDivider; // calculate the amount of y-axis rotation
		gCameraXRotVel = -(y - gMouseY) / gCameraRotDivider; // calculate the amount of x-axis rotation
		gCameraYRot += gCameraYRotVel; // apply the amount of y-axis rotation to the y-axis rotation
		gCameraXRot += gCameraXRotVel; // apply the amount of x-axis rotation to the x-axis rotation
		gMouseX = x; // update the mouse origin x position
		gMouseY = y; // update the mouse origin x position
	}
}



/*!
    @function   keyboard()
    @abstract   GLUT keyboard event callback function; keeps track of when a key is pressed
    @param      unsigned char key: the resulting character from the key combination that was pressed
    @param      int x: the mouse's x coordinate when the button was pressed (not used by this program)
    @param      int y: the mouse's y coordinate when the button was pressed (not used by this program)
*/
void keyboard(unsigned char key, int x, int y)
{
	switch (key) { // check which key was pressed
		case '-':
		case '_':
			gSpeedMultiplier /= 2.0f; // if the minus key was pressed, cut simulation speed in half
			break;
		case '=':
		case '+':
			gSpeedMultiplier *= 2.0f; // if the plus key was pressed, double the simulation speed
			break;
		case ' ':
		case 'p':
		case 'P':
			gSpeedPause = !gSpeedPause; // if the space bar or the P key was pressed, pause or unpause the simulation speed
			break;
		case 'r':
		case 'R':
			gSpeedMultiplier = -gSpeedMultiplier; // if the R key was pressed, reverse the simulation speed
			break;
		case 'f':
		case 'F':
			gIsFullscreen = !gIsFullscreen;
			if (gIsFullscreen)
				glutFullScreen();
			else
				glutPositionWindow(0, 0);
			break;
		/*case 'k':
		case 'K':
			gWaitForKey = !gWaitForKey;
			break;*/
	}
}



/*!
    @function   update()
    @abstract   GLUT idle callback function; sets glViewport and glOrtho depending on how the window was resized
    @param      int width: the newly resized width
    @param      int height: the newly resized height
*/
void update(int value)
{
	static GLfloat lastUpdateTime = get_secs(); // the number of ticks the last time update was run
	float elapsedSecs = get_secs() - lastUpdateTime; // total elapsed seconds since the last update
	float multiplier = elapsedSecs * gSpeedMultiplier; // combines the elapsed seconds multiplier and the speed multiplier into 1 multiplier used below
	bounceSides bounceResult;
	
	
	// update positions of scene objects
	
	if (!gSpeedPause) // if we are not currently paused
	{
		//gSpeedMultiplier *= 1.01f; // accelerate
		
		// cube
		// add the velocity (multipler-ified) to the position
		gCubePos[kX] += gCubeVel[kX] * multiplier;
		gCubePos[kY] += gCubeVel[kY] * multiplier;
		gCubePos[kZ] += gCubeVel[kZ] * multiplier;
		//gCubeVel[1] -= 100.0f * multiplier; // gravity
		// compute bouncing off of edges of bounding box
		bounceResult = bounceCheck(gCubePos, gCubeVel, gCubeSize);
		gCubeRot[kX] += gCubeRotDel[kX] * multiplier;
		gCubeRot[kY] += gCubeRotDel[kY] * multiplier;
		gCubeRot[kZ] += gCubeRotDel[kZ] * multiplier;
		if (gCubeRot[kX] > 180) // check angle
			gCubeRot[kX] -= 360; // wrap around if angle goes past 180¡
		if (gCubeRot[kX] < -180) // check angle
			gCubeRot[kX] += 360; // wrap around if angle goes past -180¡
		if (gCubeRot[kY] > 180) // check angle
			gCubeRot[kY] -= 360; // wrap around if angle goes past 180¡
		if (gCubeRot[kY] < -180) // check angle
			gCubeRot[kY] += 360; // wrap around if angle goes past -180¡
		if (gCubeRot[kZ] > 180) // check angle
			gCubeRot[kZ] -= 360; // wrap around if angle goes past 180¡
		if (gCubeRot[kZ] < -180) // check angle
			gCubeRot[kZ] += 360; // wrap around if angle goes past -180¡
		
		// pyramid
		// add the velocity (multipler-ified) to the position
		gPyramidPos[kX] += gPyramidVel[kX] * multiplier;
		gPyramidPos[kY] += gPyramidVel[kY] * multiplier;
		gPyramidPos[kZ] += gPyramidVel[kZ] * multiplier;
		//gPyramidVel[1] -= 100.0f * multiplier; // gravity
		// compute bouncing off of edges of bounding box
		bounceResult = bounceCheck(gPyramidPos, gPyramidVel, gPyramidSize);
		gPyramidRot[kX] += gPyramidRotDel[kX] * multiplier;
		gPyramidRot[kY] += gPyramidRotDel[kY] * multiplier;
		gPyramidRot[kZ] += gPyramidRotDel[kZ] * multiplier;
		if (gPyramidRot[kX] > 180) // check angle
			gPyramidRot[kX] -= 360; // wrap around if angle goes past 180¡
		if (gPyramidRot[kX] < -180) // check angle
			gPyramidRot[kX] += 360; // wrap around if angle goes past -180¡
		if (gPyramidRot[kY] > 180) // check angle
			gPyramidRot[kY] -= 360; // wrap around if angle goes past 180¡
		if (gPyramidRot[kY] < -180) // check angle
			gPyramidRot[kY] += 360; // wrap around if angle goes past -180¡
		if (gPyramidRot[kZ] > 180) // check angle
			gPyramidRot[kZ] -= 360; // wrap around if angle goes past 180¡
		if (gPyramidRot[kZ] < -180) // check angle
			gPyramidRot[kZ] += 360; // wrap around if angle goes past -180¡
	}
	
	
	// camera movement
	
	// calculate angle around center
	if (kCameraCoasting && !gIsRotating) { // if we are coasting from a spin by the user
		gCameraYRot += gCameraYRotVel; // apply the amount of y-axis rotation velocity to the y-axis rotation
		gCameraXRot += gCameraXRotVel; // apply the amount of x-axis rotation velocity to the x-axis rotation
	}
	
	if (gCameraYRot > M_PI) // check angle
		gCameraYRot -= 2 * M_PI; // wrap around if angle goes past ¹
	if (gCameraYRot < -M_PI) // check angle
		gCameraYRot += 2 * M_PI; // wrap around if angle goes past -¹
	
	if (gCameraXRot >= M_PI) // check angle
		gCameraXRot = M_PI - 0.0001f;
	if (gCameraXRot <= 0.0f) // check angle
		gCameraXRot = 0.0001f;
	
	gCameraPos[kX] = /* = adjacent/hypotenuse */ cos(gCameraYRot) /* ¥ hypotenuse */ * gCameraAltitude /* = adjacent */;
	gCameraPos[kY] = /* = opposite/hypotenuse */ sin(gCameraYRot) /* ¥ hypotenuse */ * gCameraAltitude /* = adjacent */;
	gCameraPos[kZ] = /* = adjacent/hypotenuse */ cos(gCameraXRot) /* ¥ hypotenuse */ * gCameraAltitude /* = adjacent */;
	GLfloat tempXRotZMult = /* = opposite/hypotenuse */ sin(gCameraXRot) /* ¥ hypotenuse */;
	gCameraPos[kY] *= tempXRotZMult;
	gCameraPos[kX] *= tempXRotZMult;
	
	
	// finish up the update
	lastUpdateTime = get_secs(); // set this update time to the "last update" for the next time update is called
	glutPostRedisplay();
	if (value) // if this timer was set up in this update function,
		glutTimerFunc(20, update, 1); // set up another timer callback
}



/*!
    @function   display()
    @abstract   GLUT window display callback function; redraws everything in the window
*/
void display(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
	gluLookAt(/* camera at:   */ gCameraPos[kX], gCameraPos[kY], gCameraPos[kZ],
			  /* aim towards: */ 0.0f, 0.0f, 0.0f,
			  /* up is:       */ 0.0f, 0.0f, 1.0f);
	
	
	// update light positions
	glLightfv(GL_LIGHT0, GL_POSITION, gLight0Pos);
	glLightfv(GL_LIGHT1, GL_POSITION, gLight1Pos);
	glLightfv(GL_LIGHT2, GL_POSITION, gLight2Pos);
	
	
	// set up the material values
	GLfloat ambient[]	= {0.5f, 0.5f, 0.5f, 1.0f};
	GLfloat diffuse[]	= {0.5f, 0.5f, 0.5f, 1.0f};
	GLfloat specular[]	= {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat emission[]	= {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat shine		= 128.0f;
	
	// set the material
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	glMaterialf(GL_FRONT, GL_SHININESS, shine);
	
	
	drawAxes();
	
	
	// draw the texture map by itself, make sure it wasn't deleted in init()
	//glRasterPos3f(0.0f, 0.0f, 0.0f);
	//glDrawPixels(16, 256, GL_RGBA, GL_UNSIGNED_BYTE, gTexData);
	
	
	// bounding box: figure out position and draw
	glColor3f(0.5f, 0.5f, 0.5f);
	glPushMatrix();
	glScalef(gBoundBox[1] - gBoundBox[0], gBoundBox[3] - gBoundBox[2], gBoundBox[4] - gBoundBox[5]);
	GLfloat kCube[6] = {-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f};
	glutWireCube(1.0f);
	glPopMatrix();
	
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	//glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	//draw3DCubes();
	draw3DLayers();
	glPopMatrix();
	
	
	// pyramid: figure out position and draw
	/*glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef(gPyramidPos[kX], gPyramidPos[kY], gPyramidPos[kZ]);
	glRotatef(gPyramidRot[kX], 1.0f, 0.0f, 0.0f);
	glRotatef(gPyramidRot[kY], 0.0f, 1.0f, 0.0f);
	glRotatef(gPyramidRot[kZ], 0.0f, 0.0f, 1.0f);
	drawTexPyramid(gPyramidSize, gPyramidSize);
	glPopMatrix();*/
	
	
	// show depth information instead of color
	//drawDepthBuffer();
	
	
	glFlush();
	//glutSwapBuffers();
}



/*!
    @function   drawAxes()
    @abstract   draws colored X, Y, and Z axes; to be called from the display function
*/
void drawAxes(void)
{
	GLfloat oldLineWidth;
	glGetFloatv(GL_LINE_WIDTH, &oldLineWidth);
	GLboolean oldLighting = glIsEnabled(GL_LIGHTING);
	GLfloat axes[3][2][3] = {{{-60.0f, 0.0f, 0.0f}, {60.0f, 0.0f, 0.0f}},
							 {{0.0f, -60.0f, 0.0f}, {0.0f, 60.0f, 0.0f}},
							 {{0.0f, 0.0f, -60.0f}, {0.0f, 0.0f, 60.0f}}}; // the 3D coordinates [3] for each point [2] for each axis [3]
	
	glDisable(GL_LIGHTING);
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	
	// draw X/blue
	glColor3f(0.0f, 0.0f, 0.5f);
	glVertex3fv(axes[0][0]);
	glVertex3fv(axes[0][1]);
	
	// draw Y/green
	glColor3f(0.0f, 0.5f, 0.0f);
	glVertex3fv(axes[1][0]);
	glVertex3fv(axes[1][1]);
	
	// draw Z/red
	glColor3f(0.5f, 0.0f, 0.0f);
	glVertex3fv(axes[2][0]);
	glVertex3fv(axes[2][1]);
	
	glEnd();
	glLineWidth(oldLineWidth);
	if (oldLighting)
		glEnable(GL_LIGHTING);
}



/*!
    @function   draw3DLayers()
    @abstract   
*/
void draw3DLayers(void)
{
	GLfloat verticesZ[4][2] = {{-30.0f, -60.0f}, {30.0f, -60.0f}, {30.0f, 60.0f}, {-30.0f, 60.0f}};
	GLfloat verticesY[4][2] = {{-30.0f, -60.0f}, {30.0f, -60.0f}, {30.0f, 60.0f}, {-30.0f, 60.0f}};
	GLfloat verticesX[4][2] = {{-60.0f, -60.0f}, {60.0f, -60.0f}, {60.0f, 60.0f}, {-60.0f, 60.0f}};
	GLfloat texCoords[4][2] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};
	
	// draw
	glEnable(GL_TEXTURE_3D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_3D, gTex);
	
	
	// Z
	
	for (int layerI = 0; layerI < 120; ++layerI) { // loop through each face we have to draw
		glNormal3f(0.0f, 0.0f, 1.0f);
		
		glBegin(GL_POLYGON);
		for (int vertexI = 0; vertexI < 4; ++vertexI) { // loop through each vertex on this face until we hit the last one or a NULL one
			glTexCoord3f(texCoords[vertexI][kS], texCoords[vertexI][kT], (GLfloat)layerI / 119.0f); // map the texture to the 4 vertices
			glVertex3f(verticesZ[vertexI][kX], verticesZ[vertexI][kY], (GLfloat)layerI / 119.0f * 120.0f - 60.0f); // plot the vertex
		}
		glEnd();
	} // end loop through each face
	
	
	for (int layerI = 119; layerI >= 0; --layerI) { // loop through each face we have to draw
		glNormal3f(0.0f, 0.0f, -1.0f);
		
		glBegin(GL_POLYGON);
		for (int vertexI = 3; vertexI >= 0; --vertexI) { // loop through each vertex on this face until we hit the last one or a NULL one
			glTexCoord3f(texCoords[vertexI][kS], texCoords[vertexI][kT], (GLfloat)layerI / 119.0f); // map the texture to the 4 vertices
			glVertex3f(verticesZ[vertexI][kX], verticesZ[vertexI][kY], (GLfloat)layerI / 119.0f * 120.0f - 60.0f); // plot the vertex
		}
		glEnd();
	} // end loop through each face
	
	
	// Y
	
	for (int layerI = 0; layerI < 120; ++layerI) { // loop through each face we have to draw
		glNormal3f(0.0f, 1.0f, 0.0f);
		
		glBegin(GL_POLYGON);
		for (int vertexI = 0; vertexI < 4; ++vertexI) { // loop through each vertex on this face until we hit the last one or a NULL one
			glTexCoord3f(texCoords[vertexI][kS], (GLfloat)layerI / 119.0f, texCoords[vertexI][kT]); // map the texture to the 4 vertices
			glVertex3f(verticesY[vertexI][0], (GLfloat)layerI / 119.0f * 120.0f - 60.0f, verticesY[vertexI][1]); // plot the vertex
		}
		glEnd();
	} // end loop through each face
	
	
	for (int layerI = 119; layerI >= 0; --layerI) { // loop through each face we have to draw
		glNormal3f(0.0f, -1.0f, 0.0f);
		
		glBegin(GL_POLYGON);
		for (int vertexI = 3; vertexI >= 0; --vertexI) { // loop through each vertex on this face until we hit the last one or a NULL one
			glTexCoord3f(texCoords[vertexI][kS], (GLfloat)layerI / 119.0f, texCoords[vertexI][kT]); // map the texture to the 4 vertices
			glVertex3f(verticesY[vertexI][0], (GLfloat)layerI / 119.0f * 120.0f - 60.0f, verticesY[vertexI][1]); // plot the vertex
		}
		glEnd();
	} // end loop through each face
	
	
	// X
	
	for (int layerI = 0; layerI < 60; ++layerI) { // loop through each face we have to draw
		glNormal3f(1.0f, 0.0f, 0.0f);
		
		glBegin(GL_POLYGON);
		for (int vertexI = 0; vertexI < 4; ++vertexI) { // loop through each vertex on this face until we hit the last one or a NULL one
			glTexCoord3f((GLfloat)layerI / 59.0f, texCoords[vertexI][kS], texCoords[vertexI][kT]); // map the texture to the 4 vertices
			glVertex3f((GLfloat)layerI / 59.0f * 60.0f - 30.0f, verticesX[vertexI][0], verticesX[vertexI][1]); // plot the vertex
		}
		glEnd();
	} // end loop through each face
	
	
	for (int layerI = 59; layerI >= 0; --layerI) { // loop through each face we have to draw
		glNormal3f(-1.0f, 0.0f, 0.0f);
		
		glBegin(GL_POLYGON);
		for (int vertexI = 3; vertexI >= 0; --vertexI) { // loop through each vertex on this face until we hit the last one or a NULL one
			glTexCoord3f((GLfloat)layerI / 59.0f, texCoords[vertexI][kS], texCoords[vertexI][kT]); // map the texture to the 4 vertices
			glVertex3f((GLfloat)layerI / 59.0f * 60.0f - 30.0f, verticesX[vertexI][0], verticesX[vertexI][1]); // plot the vertex
		}
		glEnd();
	} // end loop through each face
	
	glDisable(GL_TEXTURE_3D);
}



/*!
    @function   draw3DCubes()
    @abstract   
*/
void draw3DCubes(void)
{
	GLfloat test;
	
	// draw
	int depthChange = (gCameraPos[kZ] > 0.0f)?1:-1;
	int depthCount = 120;
	int depthStart = (gCameraPos[kZ] > 0.0f)?0:15;
	int depthSplit = gCameraPos[kZ] - 0.5f + 8.0f;
	if (depthSplit > depthCount - 1)  depthSplit = depthCount - 1;
	if (depthSplit < 0)  depthSplit = 0;
	
	int heightChange = (gCameraPos[kY] > 0.0f)?1:-1;
	int heightCount = 120;
	int heightStart = (gCameraPos[kY] > 0.0f)?0:15;
	int heightSplit = gCameraPos[kY] - 0.5f + 8.0f;
	if (heightSplit > heightCount - 1)  heightSplit = heightCount - 1;
	if (heightSplit < 0)  heightSplit = 0;
	
	int widthChange = (gCameraPos[kX] > 0.0f)?1:-1;
	int widthCount = 60;
	int widthStart = (gCameraPos[kX] > 0.0f)?0:15;
	int widthSplit = gCameraPos[kX] - 0.5f + 8.0f;
	if (widthSplit > widthCount - 1)  widthSplit = widthCount - 1;
	if (widthSplit < 0)  widthSplit = 0;
	
	for (int depthI = 0; depthI < depthSplit; ++depthI) { // draw low depth cubes
		for (int heightI = 0; heightI < heightSplit; ++heightI) { // draw low height cubes
			for (int widthI = 0; widthI < widthSplit; ++widthI) // draw low width cubes
				drawCube(((GLubyte (*)[120][60][4])gTexData)[depthI][heightI][widthI], widthI, heightI, depthI);
			
			for (int widthI = widthCount - 1; widthI >= widthSplit; --widthI) // draw high width cubes
				drawCube(((GLubyte (*)[120][60][4])gTexData)[depthI][heightI][widthI], widthI, heightI, depthI);
		}
		
		for (int heightI = heightCount - 1; heightI >= heightSplit; --heightI) { // draw high height cubes
			for (int widthI = 0; widthI < widthSplit; ++widthI) // draw low width cubes
				drawCube(((GLubyte (*)[120][60][4])gTexData)[depthI][heightI][widthI], widthI, heightI, depthI);
			
			for (int widthI = widthCount - 1; widthI >= widthSplit; --widthI) // draw high width cubes
				drawCube(((GLubyte (*)[120][60][4])gTexData)[depthI][heightI][widthI], widthI, heightI, depthI);
		}
	}
	
	for (int depthI = depthCount - 1; depthI >= depthSplit; --depthI) { // draw high depth cubes
		for (int heightI = 0; heightI < heightSplit; ++heightI) { // draw low height cubes
			for (int widthI = 0; widthI < widthSplit; ++widthI) // draw low width cubes
				drawCube(((GLubyte (*)[120][60][4])gTexData)[depthI][heightI][widthI], widthI, heightI, depthI);
			
			for (int widthI = widthCount - 1; widthI >= widthSplit; --widthI) // draw high width cubes
				drawCube(((GLubyte (*)[120][60][4])gTexData)[depthI][heightI][widthI], widthI, heightI, depthI);
		}
		
		for (int heightI = heightCount - 1; heightI >= heightSplit; --heightI) { // draw high height cubes
			for (int widthI = 0; widthI < widthSplit; ++widthI) // draw low width cubes
				drawCube(((GLubyte (*)[120][60][4])gTexData)[depthI][heightI][widthI], widthI, heightI, depthI);
			
			for (int widthI = widthCount - 1; widthI >= widthSplit; --widthI) // draw high width cubes
				drawCube(((GLubyte (*)[120][60][4])gTexData)[depthI][heightI][widthI], widthI, heightI, depthI);
		}
	}
}



/*!
    @function   drawCube()
    @abstract   draws a 3D cube of the specified color for the specified width, height, and depth iterators
    @param      GLubyte *color: pointer to the first GLubyte of a 4-GLubyte RGBA color
    @param      GLfloat widthI: the width iterator
    @param      GLfloat heightI: the height iterator
    @param      GLfloat depthI: the depth iterator
*/
void drawCube(GLubyte *color, int widthI, int heightI, int depthI)
{
	if (color[3] == 0x00)
		return;
	
	/**char junk;
	if (gWaitForKey)
		std::cin.get(junk);
	if (junk == 'k' || junk == 'K')
		gWaitForKey = false;*/
	
	glColor4ubv(color);
	glPushMatrix();
	glTranslatef((GLfloat)widthI - 8.0f + 0.5f,
				 (GLfloat)heightI - 8.0f + 0.5f,
				 (GLfloat)depthI - 8.0f + 0.5f);
	glutSolidCube(1.0f);
	glPopMatrix();
	
	//glFlush();
}



/*!
    @function   drawTexCube()
    @abstract   draws a textured 3D cube of the specified size
    @param      GLfloat size: the width, height, and depth of the cube to be drawn
*/
/*void drawTexCube(GLfloat size)
{
	GLfloat vertices[8][3];
	GLfloat *faces[6][4];
	GLfloat tempNormal[3];
	GLfloat texRectCoords[4][2] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};
	
	// calculate shape
	vertices[0][kY] = vertices[1][kY] = vertices[2][kY] = vertices[3][kY] = size / 2.0f; // top
	vertices[4][kY] = vertices[5][kY] = vertices[6][kY] = vertices[7][kY] = -size / 2.0f; // bottom
	
	vertices[0][kZ] = vertices[3][kZ] = vertices[4][kZ] = vertices[7][kZ] = size / 2.0f; // front
	vertices[1][kZ] = vertices[2][kZ] = vertices[5][kZ] = vertices[6][kZ] = -size / 2.0f; // back
	
	vertices[0][kX] = vertices[1][kX] = vertices[6][kX] = vertices[7][kX] = size / 2.0f; // right
	vertices[2][kX] = vertices[3][kX] = vertices[4][kX] = vertices[5][kX] = -size / 2.0f; // left
	
	faces[0][0] = vertices[3];	faces[0][1] = vertices[0];	faces[0][2] = vertices[1];	faces[0][3] = vertices[2]; // top side
	faces[1][0] = vertices[5];	faces[1][1] = vertices[6];	faces[1][2] = vertices[7];	faces[1][3] = vertices[4]; // bottom side
	faces[2][0] = vertices[4];	faces[2][1] = vertices[7];	faces[2][2] = vertices[0];	faces[2][3] = vertices[3]; // front side
	faces[3][0] = vertices[2];	faces[3][1] = vertices[1];	faces[3][2] = vertices[6];	faces[3][3] = vertices[5]; // back side
	faces[4][0] = vertices[7];	faces[4][1] = vertices[6];	faces[4][2] = vertices[1];	faces[4][3] = vertices[0]; // right side
	faces[5][0] = vertices[5];	faces[5][1] = vertices[4];	faces[5][2] = vertices[3];	faces[5][3] = vertices[2]; // left side
	
	// draw
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, gPaperTex);
	for (int faceI = 0; faceI < 6; ++faceI) { // loop through each face we have to draw
		glNormal3fv(computeNormal(faces[faceI][0], faces[faceI][1], faces[faceI][2], tempNormal)); // compute normal based on 1st 3 points
		
		glBegin(GL_POLYGON);
		for (int vertexI = 0; vertexI < 4 && faces[faceI][vertexI]; ++vertexI) { // loop through each vertex on this face until we hit the last one or a NULL one
			glTexCoord2f(texRectCoords[vertexI][kS], texRectCoords[vertexI][kT]); // map the texture to the 4 vertices
			glVertex3fv(faces[faceI][vertexI]); // plot the vertex
		}
		glEnd();
	} // end loop through each face
	glDisable(GL_TEXTURE_2D);
}*/



/*!
    @function   bounceCheck()
    @abstract   checks to see if the object is colliding with the bounding box in the X, Y, or Z axes and bounces it if so
    @param      GLfloat objPos[]: the 3 coordinate position of the object to be bounced
    @param      GLfloat objVel[]: the 3 coordinate velocity of the object to be bounced
    @param      GLfloat objSize: the single coordinate size of the object to be bounced
    @param      GLfloat objSize[]: the 3 coordinate size of the object to be bounced
	@return		bounceSides: the side(s) the object bounced on
*/
bounceSides bounceCheck(GLfloat objPos[], GLfloat objVel[], GLfloat objSize)
{
	GLfloat objSize3[3] = {objSize, objSize, objSize};
	return bounceCheck(objPos, objVel, objSize3);
}

bounceSides bounceCheck(GLfloat objPos[], GLfloat objVel[], GLfloat objSize[])
{
	bounceSides result = {0, 0, 0};
	
	if (objPos[0] - objSize[0] / 2.0f < gBoundBox[0]) { // if the object's left/-X side is less than the left/-X side of the bounding box,
		objPos[0] += gBoundBox[0] - (objPos[0] - objSize[0] / 2.0f); // add the difference to the object's X coordinate
		//objPos[0] = gBoundBox[0] + (objSize[0] / 2.0f);
		objVel[0] = -objVel[0]; // reverse the object's X velocity
		result.arr[0] = -1;
	}
	if (objPos[0] + objSize[0] / 2.0f > gBoundBox[1]) { // if the object's right/+X side is greater than the right/+X side of the bounding box,
		objPos[0] -= (objPos[0] + objSize[0] / 2.0f) - gBoundBox[1]; // subtract the difference from the object's X coordinate
		//objPos[0] = gBoundBox[1] - (objSize[0] / 2.0f);
		objVel[0] = -objVel[0]; // reverse the object's X velocity
		result.arr[0] = 1;
	}
	if (objPos[1] - objSize[1] / 2.0f < gBoundBox[2]) { // if the object's bottom/-Y side is less than the bottom/-Y side of the bounding box,
		objPos[1] += gBoundBox[2] - (objPos[1] - objSize[1] / 2.0f); // add the difference to the object's Y coordinate
		//objPos[1] = gBoundBox[2] + (objSize[1] / 2.0f);
		objVel[1] = -objVel[1]; // reverse the object's Y velocity
		result.arr[1] = -1;
	}
	if (objPos[1] + objSize[1] / 2.0f > gBoundBox[3]) { // if the object's top/+Y side is greater than the top/+Y side of the bounding box,
		objPos[1] -= (objPos[1] + objSize[1] / 2.0f) - gBoundBox[3]; // subtract the difference from the object's Y coordinate
		//objPos[1] = gBoundBox[3] - (objSize[1] / 2.0f);
		objVel[1] = -objVel[1]; // reverse the object's Y velocity
		result.arr[1] = 1;
	}
	if (objPos[2] - objSize[2] / 2.0f < gBoundBox[5]) { // if the object's back/-Z side is less than the back/-Z side of the bounding box,
		objPos[2] += gBoundBox[5] - (objPos[2] - objSize[2] / 2.0f); // add the difference to the object's Z coordinate
		//objPos[2] = gBoundBox[5] + (objSize[2] / 2.0f);
		objVel[2] = -objVel[2]; // reverse the object's Z velocity
		result.arr[2] = -1;
	}
	if (objPos[2] + objSize[2] / 2.0f > gBoundBox[4]) { // if the object's front/+Z side is greater than the front/+Z side of the bounding box,
		objPos[2] -= (objPos[2] + objSize[2] / 2.0f) - gBoundBox[4]; // subtract the difference from the object's Z coordinate
		//objPos[2] = gBoundBox[4] - (objSize[2] / 2.0f);
		objVel[2] = -objVel[2]; // reverse the object's Z velocity
		result.arr[2] = 1;
	}
	
	return result;
}



/*!
    @function   drawDepthBuffer()
    @abstract   copies the depth buffer to the color buffer; call from display() after drawing everything
*/
void drawDepthBuffer(void)
{
	int		x, y;
    GLfloat	max, min;
    GLint	previousColorBuffer;
	GLubyte	*depthSave = (GLubyte *)malloc(gWinWidth * gWinHeight * 4 * sizeof(GLubyte));

    glReadPixels(0, 0, gWinWidth, gWinHeight, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, depthSave);

    // I'm sure this could be done much better with OpenGL
    max = 0;
    min = 255;
    for (y = 0; y < gWinHeight; ++y)
	for (x = 0; x < gWinWidth; ++x) {
	    if (depthSave[gWinWidth * y + x] < min)
		min = depthSave[gWinWidth * y + x];
	    if (depthSave[gWinWidth * y + x] > max && depthSave[gWinWidth * y + x] < 254)
		max = depthSave[gWinWidth * y + x];
	}

    for (y = 0; y < gWinHeight; ++y)
	for (x = 0; x < gWinWidth; ++x) {
	    if(depthSave[gWinWidth * y + x] <= max)
		depthSave[gWinWidth * y + x] = 255 - 255 * (GLfloat)(depthSave[gWinWidth * y + x] - min) / (max - min);
	    else
		depthSave[gWinWidth * y + x] = 0;
	}

    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 1, 0, 1, 0, 1);
	
    glRasterPos3f(0, 0, -.5);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glGetIntegerv(GL_DRAW_BUFFER, &previousColorBuffer);
    glDrawBuffer(GL_DRAW_BUFFER);
    glDrawPixels(gWinWidth, gWinHeight, GL_LUMINANCE , GL_UNSIGNED_BYTE, depthSave);
    glDrawBuffer(previousColorBuffer);
    glEnable(GL_DEPTH_TEST);
	
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
	
	free(depthSave);
}
