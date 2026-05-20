#include "myImageIO.h"
#include "histogram.hpp"
#include <stdexcept>

enum class HistMode { Grey, Color_Luminance, Color_Distorted };

int* map_table(const int* hist_data, const int total_pixel) {

	int* table = new int[256];

	// cdf element
	int sum = 0;
	for (int level = 0; level < 256; level++) {
		sum += hist_data[level];
		// normalize histogram
		double r = sum / (double)total_pixel;

		// scaling up
		table[level] = (unsigned char)(255.0 * r);
	}

	return table;
}

void histogram_equalization(myImageData* img1, myImageData* img2,
							const HistMode mode = HistMode::Color_Luminance) {

	// get size
	int W = img1->getWidth();
	int H = img1->getHeight();
	int CH = img1->getCH();

	// total pixel
	const int total_pixel = W * H;

	int* hist;
	if (CH == 1) {
		hist = Histogram(img1, 1, false);

		const auto table = map_table(hist, total_pixel);

		// apply to the output img
		for (int y = 0; y < H; y++) {
			for (int x = 0; x < W; x++) {
				int value = img1->get(x, y);
				img2->set(x, y, table[value]);
			}
		}
		delete[] table;
		return;
	} else if (CH == 3 && mode == HistMode::Color_Luminance) {
		hist = std::get<int*>(Histogram_color(img1, 1, true));
		const auto table = map_table(hist, total_pixel);

		// apply to the output img
		for (int y = 0; y < H; y++) {
			for (int x = 0; x < W; x++) {
				// convert RGB <-> YUV function by myself
				RGB rgb_value = img1->get_RGB(x, y);
				YUV yuv_value = convert_rgb_to_yuv(rgb_value);

				yuv_value.Y = table[(int)yuv_value.Y];
				RGB new_rgb_value = convert_yuv_to_rgb(yuv_value);
				// use set with RGB overload (added)
				img2->set(x, y, new_rgb_value);
			}
		}
		delete[] table;
		return;
	} else if (CH == 3 && mode == HistMode::Color_Distorted) {
		const auto hist_rgb =
			std::get<HistogramRGB>(Histogram_color(img1, 1, false));
		const auto hist_R = hist_rgb.hist_data_R;
		const auto hist_G = hist_rgb.hist_data_G;
		const auto hist_B = hist_rgb.hist_data_B;

		const auto table_R = map_table(hist_R, total_pixel);
		const auto table_G = map_table(hist_G, total_pixel);
		const auto table_B = map_table(hist_B, total_pixel);

		// apply to the output img
		for (int y = 0; y < H; y++) {
			for (int x = 0; x < W; x++) {
				// convert RGB <-> YUV function by myself
				RGB rgb_value = img1->get_RGB(x, y);

				rgb_value.R = table_R[(int)rgb_value.R];
				rgb_value.G = table_G[(int)rgb_value.G];
				rgb_value.B = table_B[(int)rgb_value.B];

				// use set with RGB overload (added)
				img2->set(x, y, rgb_value);
			}
		}
		delete[] table_R;
		delete[] table_G;
		delete[] table_B;
	} else {
		throw std::runtime_error("Invalid channel size.");
	}
}

int main(int argc, char** argv) {
	// read image data to img1

	myImageData* img1 = new myImageData();
	img1->read(argv[1]);

	int W = img1->getWidth();
	int H = img1->getHeight();
	int CH = img1->getCH();

	printf("channel: %d\n", CH);

	myImageData* img2 = new myImageData();
	img2->init(W, H, CH);

	// histogram_equalization(img1, img2, HistMode::Grey);
	// img2->save("histogram_equalizer");

	histogram_equalization(img1, img2, HistMode::Color_Luminance);
	img2->save("histogram_equalizer_color");
	histogram_equalization(img1, img2, HistMode::Color_Distorted);
	img2->save("histogram_equalizer_color_distorted");

	delete img1;
	delete img2;

	return 1; // normal termination
}