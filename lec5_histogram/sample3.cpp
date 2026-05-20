#include <stdio.h>
#include <stdlib.h>
#include "myImageIO.h"
#include "histogram.hpp"


int main(int argc, char **argv){
	
	// read image data to img1
	
	myImageData * img1 = new myImageData();
	img1->read(argv[1]);

	int W = img1->getWidth();
	int H = img1->getHeight();
	
	Histogram(img1, 8);
	
	delete img1;
	
	return 1; // normal termination
	
}