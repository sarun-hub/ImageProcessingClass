#include "binarization.hpp"
#include <stdio.h>
#include <string>

int main(int argc, char** argv) {

	myImageData* img1 = new myImageData();

	img1->read(argv[1]);
	const auto& H = img1->getHeight();
	const auto& W = img1->getWidth();
	const auto& CH = img1->getCH();

	printf("resolution: %d x %d\n", W, H);
	printf("channel: %d\n", CH);

	myImageData* img2 = new myImageData();
	img2->init(W, H, CH);

	// processing

	simple_thresholding(img1, img2, 122.5);
	const char* save_filename_simple_thresholding =
		CH == 3 ? "simple_thresholding_color" : "simple_thresholding";
	img2->save(save_filename_simple_thresholding);

	error_diffusion(img1, img2, 122.5);
	const char* save_filename_error_diffusion =
		CH == 3 ? "error_diffusion_color" : "error_diffusion";
	img2->save(save_filename_error_diffusion);
}