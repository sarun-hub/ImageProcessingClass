#include "myImageIO.h"

inline void simple_thresholding(myImageData* img1, myImageData* img2,
								const double threshold = 122.5) {
	const auto H = img1->getHeight();
	const auto W = img1->getWidth();
	const auto CH = img1->getCH();

	for (int y = 0; y < H; y++) {
		for (int x = 0; x < W; x++) {
			for (int channel = 0; channel < CH; channel++) {
				const auto& value = img1->get(x, y, channel);

				if (value <= threshold) {
					img2->set(x, y, channel, 0);
				} else {
					img2->set(x, y, channel, 255);
				}
			}
		}
	}
}

inline void error_diffusion(myImageData* img1, myImageData* img2,
							const double threshold = 122.5) {
	const auto H = img1->getHeight();
	const auto W = img1->getWidth();
	const auto CH = img1->getCH();

    // copy the image
    *img2 = *img1;

	for (int y = 0; y < H; y++) {
		for (int x = 0; x < W; x++) {
			for (int channel = 0; channel < CH; channel++) {
				const auto& value = img1->get(x, y, channel);

				double output_value = value > threshold ? 255.0 : 0.0;
				img2->set(x, y, channel, output_value);

				double error = value - output_value;

				// error propagation
				if (x + 1 < W) {
					img2->set(x + 1, y, channel,
							  img2->get(x + 1, y, channel) +
								  7.0 / 16.0 * error);
				}
				if (x - 1 > 0 && y + 1 < H) {
					img2->set(x - 1, y + 1, channel,
							  img2->get(x - 1, y + 1, channel) +
								  3.0 / 16.0 * error);
				}
				if (y + 1 < H) {
					img2->set(x, y + 1, channel,
							  img2->get(x, y + 1, channel) +
								  5.0 / 16.0 * error);
				}
				if (x + 1 < W && y + 1 < H) {
					img2->set(x + 1, y + 1, channel,
							  img2->get(x + 1, y + 1, channel) +
								  1.0 / 16.0 * error);
				}
			}
		}
	}
}