/**
 * <p>Title: utility function code file</p>
 * <p>Description: CS 337 Assignment # 4</p>
 * <p>Copyright: Copyright © 2005</p>
 * <p>Company: UW Ñ River Falls</p>
 * @author Kyle Thompson
 * @email kyle.thompson@uwrf.edu
 * @date November 11th, 2005
 * @team 
 * @version 4.0
 */

#pragma mark includes
#include "util.h"



/*!
    @function	degF2RadF()
    @abstract	converts a float in degrees to radians
    @param		GLfloat degrees: value in degrees
    @return		GLfloat: value in radians
*/
GLfloat degF2RadF(GLfloat degrees)
{
	return degrees / 360.0f * 2.0f * M_PI; // return the angle in radians
}



/*!
    @function	radF2DegF()
    @abstract	converts a float in degrees to radians
    @param		GLfloat radians: value in radians
    @return		GLfloat: value in degrees
*/
GLfloat radF2DegF(GLfloat radians)
{
	return radians / M_PI / 2.0f * 360.0f; // return the angle in degrees
}



/*!
    @function	get_secs()
    @abstract	gets the number of seconds since program start from GLUT
    @return		GLfloat: number of seconds since program start
*/
GLfloat get_secs(void)
{
	return glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
}



/*!
    @function	length()
    @abstract	calculates the length of a vertex from the point (0, 0, 0)
    @param		GLfloat v[3]: the 3-element vector to have length computed for
    @result		GLfloat: the length computed
*/
GLfloat length(GLfloat v[3])
{
	return (GLfloat)sqrt(v[kX] * v[kX] + v[kY] * v[kY] + v[kZ] * v[kZ]);
}



/*!
    @function	normalize()
    @abstract	Returns a 3-element vector of the same length from (0, 0, 0) as vIn, just only 1 long
    @param		GLfloat vIn[3]: the 3-element vector to be normalized
    @result		GLfloat normal[3]: the variable to return the normalized 3-element vector
    @result		GLfloat *: the normalized 3-element vector
*/
GLfloat *normalize(GLfloat vIn[3], GLfloat normal[3])
{
	GLfloat len = length(vIn);
	normal[kX] = vIn[kX] / len;
	normal[kY] = vIn[kY] / len;
	normal[kZ] = vIn[kZ] / len;
	return normal;
}



/*!
    @function	computeNormal()
    @abstract	computes the normal (perpendicular 1-length vector) for a vertex based on 2 other vertices it is connected to
    @param		GLfloat v1[3]: the vertex to have the normal computed for
    @param		GLfloat v2[3]: a vertex connected to v1
    @param		GLfloat v3[3]: another vertex connected to v1
    @result		GLfloat normal[3]: the variable to return v1's normal as a vector relative to (0, 0, 0)
    @result		GLfloat *: v1's normal as a vector relative to (0, 0, 0)
*/
GLfloat *computeNormal(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3], GLfloat normal[3])
{
	GLfloat u[3], v[3];
	
	u[kX] = v2[kX] - v1[kX]; // calculate v2's X coordinate relative to v1
	u[kY] = v2[kY] - v1[kY]; // calculate v2's Y coordinate relative to v1
	u[kZ] = v2[kZ] - v1[kZ]; // calculate v2's Z coordinate relative to v1
	v[kX] = v3[kX] - v1[kX]; // calculate v3's X coordinate relative to v1
	v[kY] = v3[kY] - v1[kY]; // calculate v3's Y coordinate relative to v1
	v[kZ] = v3[kZ] - v1[kZ]; // calculate v3's Z coordinate relative to v1
	
	// u and v are the two vectors
	normal[kX] = u[kY] * v[kZ] - u[kZ] * v[kY];
	normal[kY] = u[kZ] * v[kX] - u[kX] * v[kZ];
	normal[kZ] = u[kX] * v[kY] - u[kY] * v[kX];
	
	return normalize(normal, normal); // return the length-changed-of-the-cross-product-to-1 normal
}