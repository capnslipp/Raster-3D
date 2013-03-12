/**
 * <p>Title: file operation function header file</p>
 * <p>Description: CS 337 Assignment # 4</p>
 * <p>Copyright: Copyright © 2005</p>
 * <p>Company: UW Ñ River Falls</p>
 * @author Kyle Thompson
 * @email kyle.thompson@uwrf.edu
 * @date November 11th, 2005
 * @team 
 * @version 2.0
 */

#ifndef _FILE_H_
#define _FILE_H_


#pragma mark includes
#include <OpenGL/gl.h>
#include <fstream>
#include <iostream>


#pragma mark function prototypes
GLubyte *initRGBImageDataFromFile(char *filename, const int width, const int height);
GLubyte *initRGBAImageDataFromFile(char *filename, const int width, const int height);
GLubyte *initRGBAImageDataFromFile(char *filename, const int width, const int height, const int depth);


#endif // _FILE_H_