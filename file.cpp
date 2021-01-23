 /**
 * <p>Title: file operation function code file</p>
 * <p>Description: CS 337 Assignment # 4</p>
 * <p>Copyright: Copyright © 2005</p>
 * <p>Company: UW Ñ River Falls</p>
 * @author Kyle Thompson
 * @email kyle.thompson@uwrf.edu
 * @date November 11th, 2005
 * @team 
 * @version 2.0
 */

#pragma mark includes
#include "file.h"



/*!
    @function	initRGBImageDataFromFile()
    @abstract	gets a series of 3 dimensional coordinates from a specified file and returns them, along with the number of vertices found
    @param		char *filename: the filename of the file to be opened; note: depending on how you run the program, this may be relative to the directory of the program or absolute to the root of the hard drive
    @param		const int width: the width of the image data
    @param		const int height: the height of the image data
    @return		GLubyte *: the image data; remember to "delete []" this!
*/
GLubyte *initRGBImageDataFromFile(char *filename, const int width, const int height)
{
	std::ifstream imageFile; // the input file
	
	GLubyte (*imageData)[3] = (GLubyte (*)[3])new GLubyte[height * width * 3];
	
	imageFile.open(filename, std::ios::binary); // open the file
	
	if (!imageFile) { // error if we couldn't open
		std::cout << "Error: File \"" << filename << "\" could not be opened.\n";
		return NULL;
	}
	
	
	for (int heightI = 0; heightI < height; ++heightI)
		for (int widthI = 0; widthI < width; ++widthI) {
			int pixelI = (heightI * width) + widthI;
			
			if (imageFile.eof()) {
				imageData[pixelI][0] = imageData[pixelI][1] = imageData[pixelI][2] = 0x00;
			} else {
				imageFile.read((char *)&imageData[pixelI][0], sizeof(GLubyte));
				imageFile.read((char *)&imageData[pixelI][1], sizeof(GLubyte));
				imageFile.read((char *)&imageData[pixelI][2], sizeof(GLubyte));
			}
		}
	
	imageFile.close(); // close the input file
	
	
	// set up the return data and size and return
	return (GLubyte *)imageData;
}



/*!
    @function	initRGBAImageDataFromFile()
    @abstract	gets a series of 3 dimensional coordinates from a specified file and returns them, along with the number of vertices found
    @param		char *filename: the filename of the file to be opened; note: depending on how you run the program, this may be relative to the directory of the program or absolute to the root of the hard drive
    @param		const int width: the width of the image data
    @param		const int height: the height of the image data
    @return		GLubyte *: the image data; remember to "delete []" this!
*/
GLubyte *initRGBAImageDataFromFile(char *filename, const int width, const int height)
{
	std::ifstream imageFile; // the input file
	
	GLubyte (*imageData)[4] = (GLubyte (*)[4])new GLubyte[height * width * 4];
	
	imageFile.open(filename, std::ios::binary); // open the file
	
	if (!imageFile) { // error if we couldn't open
		std::cout << "Error: File \"" << filename << "\" could not be opened.\n";
		return NULL;
	}
	
	
	for (int heightI = 0; heightI < height; ++heightI)
		for (int widthI = 0; widthI < width; ++widthI) {
			int pixelI = (heightI * width) + widthI;
			
			if (imageFile.eof()) {
				imageData[pixelI][0] = imageData[pixelI][1] = imageData[pixelI][2] = imageData[pixelI][3] = 0x00;
			} else {
				imageFile.read((char *)&imageData[pixelI][0], sizeof(GLubyte));
				imageFile.read((char *)&imageData[pixelI][1], sizeof(GLubyte));
				imageFile.read((char *)&imageData[pixelI][2], sizeof(GLubyte));
				imageFile.read((char *)&imageData[pixelI][3], sizeof(GLubyte));
			}
		}
	
	imageFile.close(); // close the input file
	
	
	// set up the return data and size and return
	return (GLubyte *)imageData;
}



/*!
    @function	initRGBAImageDataFromFile()
    @abstract	gets a series of 3 dimensional coordinates from a specified file and returns them, along with the number of vertices found
    @param		char *filename: the filename of the file to be opened; note: depending on how you run the program, this may be relative to the directory of the program or absolute to the root of the hard drive
    @param		const int width: the width of the image data
    @param		const int height: the height of the image data
    @return		GLubyte *: the image data; remember to "delete []" this!
*/
GLubyte *initRGBAImageDataFromFile(char *filename, const int width, const int height, const int depth)
{
	std::ifstream imageFile; // the input file
	
	GLubyte (*imageData)[4] = (GLubyte (*)[4])new GLubyte[depth * height * width * 4];
	
	imageFile.open(filename, std::ios::binary); // open the file
	
	if (!imageFile) { // error if we couldn't open
		std::cout << "Error: File \"" << filename << "\" could not be opened.\n";
		return NULL;
	}
	
	
	for (int depthI = 0; depthI < depth; ++depthI)
		for (int heightI = 0; heightI < height; ++heightI)
			for (int widthI = 0; widthI < width; ++widthI) {
				int pixelI = (depthI * width * height) + (heightI * width) + widthI;
				
				if (imageFile.eof()) {
					imageData[pixelI][0] = imageData[pixelI][1] = imageData[pixelI][2] = imageData[pixelI][3] = 0x00;
				} else {
					imageFile.read((char *)&imageData[pixelI][0], sizeof(GLubyte));
					imageFile.read((char *)&imageData[pixelI][1], sizeof(GLubyte));
					imageFile.read((char *)&imageData[pixelI][2], sizeof(GLubyte));
					imageFile.read((char *)&imageData[pixelI][3], sizeof(GLubyte));
				}
			}
	
	imageFile.close(); // close the input file
	
	
	// set up the return data and size and return
	return (GLubyte *)imageData;
}