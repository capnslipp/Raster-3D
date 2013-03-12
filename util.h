/**
 * <p>Title: utility function header file</p>
 * <p>Description: CS 337 Assignment # 4</p>
 * <p>Copyright: Copyright © 2005</p>
 * <p>Company: UW Ñ River Falls</p>
 * @author Kyle Thompson
 * @email kyle.thompson@uwrf.edu
 * @date November 11th, 2005
 * @team 
 * @version 4.0
 */

#ifndef _UTIL_H_
#define _UTIL_H_


#pragma mark includes
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <math.h>


#pragma mark defines & enums
enum axis {kX = 0, kY, kZ, kW};
enum texAxis {kS = 0, kT, kR, kQ};


#pragma mark structs
struct bounceSides {
	int arr[3];
};


#pragma mark function prototypes
GLfloat degF2RadF(GLfloat degrees);
GLfloat radF2DegF(GLfloat radians);
GLfloat get_secs(void);
GLfloat length(GLfloat v[3]);
GLfloat *normalize(GLfloat vIn[3], GLfloat normal[3]);
GLfloat *computeNormal(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3], GLfloat normal[3]);


#endif // _UTIL_H_