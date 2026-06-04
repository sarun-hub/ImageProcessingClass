#include <stdio.h>
#include <stdlib.h>
#include "myImageIO.h"
#include <vector>
#include <string>
#include <iostream>

template <typename T> using vec2d = std::vector<std::vector<T>>;

void mean_filter(myImageData* in, myImageData* out, const int winsize = 1) {

	int W = in->getWidth();
	int H = in->getHeight();
	int C = in->getCH(); // not used

	for (int y = 0; y < H; y++) {
		for (int x = 0; x < W; x++) {

			double sum = 0;
			int cnt = 0;

			// include top to bottom
			for (int j = -winsize; j <= winsize; j++) {

				int yp = y + j;
				// if go out of scope, check the opposite edge
				if (yp < 0) yp = yp + H;
				if (yp > H - 1) yp = yp - H;

				// include left to right
				for (int i = -winsize; i <= winsize; i++) {

					int xp = x + i;
					// if go out of scope, check the opposite edge
					if (xp < 0) xp = xp + W;
					if (xp > W - 1) xp = xp - W;

					double val_in = in->get(xp, yp);

					sum = sum + val_in;
					cnt = cnt + 1;
				}
			}

			double val_out = (double)sum / (double)cnt;
			out->set(x, y, val_out);
		}
	}
}

// Filtering with kernel
void filter_with_kernel(myImageData* in, myImageData* out,
						const vec2d<double>& kernel, int offset = 0) {
	int W = in->getWidth();
	int H = in->getHeight();

	// loop through all pixels
	for (int y = 0; y < H; y++) {
		for (int x = 0; x < W; x++) {
			double sum = 0;

			// following kernel size
			for (int j = 0; j < kernel.size(); j++) {
				// shift to the right kernel slot
				int yp = y + j - int(kernel.size() / 2);
				// if go out of scope, check the opposite edge
				if (yp < 0) yp = yp + H;
				if (yp > H - 1) yp = yp - H;

				for (int i = 0; i < kernel[0].size(); i++) {
					// shift to the right kernel slot
					int xp = x + i - int(kernel[0].size() / 2);
					// if go out of scope, check the opposite edge
					if (xp < 0) xp = xp + W;
					if (xp > W - 1) xp = xp - W;

					double val_in = in->get(xp, yp);

					// apply kernel value into the img data
					sum = sum + val_in * kernel[j][i];
				}
			}
			double val_out = (double)sum;
			// offset pixel by offset (0 for normal, 128 for negative related
			// filter)
			out->set(x, y, val_out + offset);
		}
	}
}

// (original) + (original) – (mean)
void detail_enhancement(myImageData* in, myImageData* out) {
	int W = in->getWidth();
	int H = in->getHeight();

	myImageData* mean_img = new myImageData();
	mean_img->init(W, H, 1);

	mean_filter(in, mean_img);

	// loop through all pixels
	for (int y = 0; y < H; y++) {
		for (int x = 0; x < W; x++) {
			double val_out = 2 * in->get(x, y) - mean_img->get(x, y);
			out->set(x, y, val_out);
		}
	}

	delete mean_img;
}

// (original) + (edge)
void detail_enhancement_edge(myImageData* in, myImageData* out,
							 vec2d<double>& kernel, int offset = 0) {
	int W = in->getWidth();
	int H = in->getHeight();

	myImageData* temp_img = new myImageData();
	temp_img->init(W, H, 1);

	filter_with_kernel(in, temp_img, kernel, offset);

	// loop through all pixels
	for (int y = 0; y < H; y++) {
		for (int x = 0; x < W; x++) {
			double val_out = in->get(x, y) + temp_img->get(x, y);
			out->set(x, y, val_out);
		}
	}

	delete temp_img;
}

int main(int argc, char** argv) {

	// read image data to img1

	myImageData* img1 = new myImageData();
	img1->read(argv[1]);

	int W = img1->getWidth();
	int H = img1->getHeight();

	printf("resolution: %d x %d\n", W, H);

	// prepare img2

	myImageData* img2 = new myImageData();
	img2->init(W, H, 1);

	// processing
	{
		vec2d<double> mean_kernel = {{1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0},
									 {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0},
									 {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0}};
		const int winsize = 1;
		mean_filter(img1, img2, winsize);
		// save
		std::string save_name = "result_mean_filter_"  + std::to_string(winsize);
		img2->save(save_name.c_str());

		filter_with_kernel(img1, img2, mean_kernel);
		img2->save("result_mean_kernel");

		// enhancement
		detail_enhancement(img1, img2);
		img2->save("result_mean_enhancement");
		detail_enhancement_edge(img1, img2, mean_kernel);
		img2->save("result_edge_enhancement");
	}

	// horizontal
	{
		vec2d<double> horizontal_sobel_kernel = {
			{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
		filter_with_kernel(img1, img2, horizontal_sobel_kernel, 128);
		// save
		img2->save("result_horizontal_kernel");

		// enhancement
		detail_enhancement_edge(img1, img2, horizontal_sobel_kernel);
		img2->save("result_horizontal_edge_enhancement");
	}

	// vertical
	{
		vec2d<double> vertical_sobel_kernel = {
			{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
		filter_with_kernel(img1, img2, vertical_sobel_kernel, 128);
		// save
		img2->save("result_vertical_kernel");

		// enhancement
		detail_enhancement_edge(img1, img2, vertical_sobel_kernel);
		img2->save("result_vertical_edge_enhancement");
	}

	// Laplacian 1
	{
		vec2d<double> laplacian1_kernel = {{1, 1, 1}, {1, -8, 1}, {1, 1, 1}};
		filter_with_kernel(img1, img2, laplacian1_kernel, 128);
		// save
		img2->save("result_laplacian1_kernel");

		// enhancement
		detail_enhancement_edge(img1, img2, laplacian1_kernel);
		img2->save("result_laplacian1_edge_enhancement");
	}

	// Laplacian 2
	{
		vec2d<double> laplacian2_kernel = {{0, 1, 0}, {1, -4, 1}, {0, 1, 0}};
		filter_with_kernel(img1, img2, laplacian2_kernel, 128);
		// save
		img2->save("result_laplacian2_kernel");

		// enhancement
		detail_enhancement_edge(img1, img2, laplacian2_kernel);
		img2->save("result_laplacian2_edge_enhancement");
	}

	delete img1;
	delete img2;

	return 0; // normal termination
}
