#include "myImageIO.h"
#include "adv_filtering.hpp"

int main(int argc, char** argv) {
	myImageData* img1 = new myImageData();

	img1->read(argv[1]);

	const auto& H = img1->getHeight();
	const auto& W = img1->getWidth();

	myImageData* img2 = new myImageData();
	img2->init(W, H, 1);

	const auto& gaussian_kernel_ = gaussian_kernel(11, 100.0, true);
	apply_kernel(img1, img2, gaussian_kernel_);

	img2->save("Gaussian filter");

	apply_kernel_with_range_weight(img1, img2, 11, 100.0, 500.0);
	img2->save("Bilateral filter");

	apply_guided_filter(img1, img2, 7, 100.0);
	img2->save("Guided filter");
}