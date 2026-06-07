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
	const double threshold = 128;
	simple_thresholding(img1, img2, threshold);
	const std::string save_filename_simple_thresholding =
		CH == 3 ? "simple_thresholding_color" + std::to_string(threshold)
				: "simple_thresholding" + std::to_string(threshold);
	img2->save(save_filename_simple_thresholding.c_str());

	error_diffusion(img1, img2, threshold);
	const std::string save_filename_error_diffusion =
		CH == 3 ? "error_diffusion_color" + std::to_string(threshold)
				: "error_diffusion" + std::to_string(threshold);
	img2->save(save_filename_error_diffusion.c_str());
}