#include <stdio.h>
#include <stdlib.h>
#include "myImageIO.h"

int main(int argc, char **argv){
	
	// read image data to img1
	
	myImageData * img1 = new myImageData();
	img1->read(argv[1]);

	int W = img1->getWidth();
	int H = img1->getHeight();
	
	printf("resolution: %d x %d\n", W, H);			
	
	// prepare img2
	
	myImageData *img2  = new myImageData();
	img2->init(W, H, 1);

	// processing

	for(int y = 0; y < H; y++){
		for(int x = 0; x < W; x++){
			
			double value = img1->get(x,y);
			img2->set(x,y,value);
			
		}
	}
		
	

	img2->save("result");	
	
	
	delete img1;
	delete img2;
	
	return 0; // normal termination
	
}