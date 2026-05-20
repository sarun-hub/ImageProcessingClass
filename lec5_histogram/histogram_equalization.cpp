#include "myImageIO.h"
#include "histogram.hpp"

void histogram_equalization(myImageData* img1, myImageData* img2) {

    Histogram(img1, 256);
}

int main(int argc, char** argv) {
	// read image data to img1

	myImageData* img1 = new myImageData();
	img1->read(argv[1]);

	int W = img1->getWidth();
	int H = img1->getHeight();

    myImageData* img2 = new myImageData();

	histogram_equalization(img1, img2);

	delete img1;

	return 1; // normal termination
}