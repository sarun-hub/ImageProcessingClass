#include <stdio.h>
#include <stdlib.h>
#include "myImageIO.h"

void mean_filter(myImageData * in, myImageData *out){

	int W = in->getWidth();
	int H = in->getHeight();
	int C = in->getCH();
	const int winsize = 5;
	
	for(int y = 0; y < H; y++){
		for(int x = 0; x < W; x++){
			
			double sum = 0;
			int cnt = 0;
			
			for(int j = -winsize; j <= winsize; j++){
		
				int yp = y + j;		
				if(yp < 0) yp = yp + H;
				if(yp > H-1) yp = yp - H;
				
				for(int i = -winsize; i <= winsize; i++){
					
					int xp = x + i;
					if(xp < 0) xp = xp + W;
					if(xp > W-1) xp = xp - W;
					
					double val_in = in->get(xp, yp);
					
					sum = sum + val_in;
					cnt = cnt + 1;
					
				}
			}
			
			double val_out = (double)sum/(double)cnt;
			out->set(x, y, val_out);
			
		}
	
	}

}

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
	
	mean_filter(img1, img2);
	
	// save 
	img2->save("result");	
	
	
	delete img1;
	delete img2;
	
	return 0; // normal termination
	
}




