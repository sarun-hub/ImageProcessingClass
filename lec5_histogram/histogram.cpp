#include "histogram.hpp"
#include "conversion.hpp"


int* Histogram(myImageData* img, int binsize, bool print_histogram) {

	int W = img->getWidth();
	int H = img->getHeight();

	int nBins = 256 / binsize;
	int* histdata = new int[nBins];

	for (int idx = 0; idx < nBins; idx++) {
		histdata[idx] = 0;
	}

	for (int y = 0; y < H; y++) {
		for (int x = 0; x < W; x++) {

			int value = img->get(x, y);
			int bin = value / binsize;
			histdata[bin]++;
		}
	}

	if (print_histogram) {
		// print the histogram

		for (int idx = 0; idx < nBins; idx++) {

			printf("[%.03d-%.03d]\t", idx * binsize, (idx + 1) * binsize - 1);

			int num = histdata[idx];
			int denom = 0.1 * W * H / nBins;
			for (int cnt = 0; cnt < num / denom; cnt++) {
				printf("*");
			}

			printf("\n");
		}
	}

	return histdata;
}

std::variant<int*, HistogramRGB> Histogram_color(myImageData* img, int binsize,
												 bool luminance_only) {

	int W = img->getWidth();
	int H = img->getHeight();
	int CH = img->getCH();

	int nBins = 256 / binsize;
	if (luminance_only) {
		int* histdata = new int[nBins];

		for (int idx = 0; idx < nBins; idx++) {
			histdata[idx] = 0;
		}

		for (int y = 0; y < H; y++) {
			for (int x = 0; x < W; x++) {

				const auto rgb_value = img->get_RGB(x, y);
				const auto yuv_value = convert_rgb_to_yuv(rgb_value);
				// use only luminance value
				int bin = yuv_value.Y / binsize;
				histdata[bin]++;
			}
		}

		return histdata;
	} else {
		int* histdata_R = new int[nBins];
		int* histdata_G = new int[nBins];
		int* histdata_B = new int[nBins];

		for (int idx = 0; idx < nBins; idx++) {
			histdata_R[idx] = 0;
			histdata_G[idx] = 0;
			histdata_B[idx] = 0;
		}

		for (int y = 0; y < H; y++) {
			for (int x = 0; x < W; x++) {

				const auto rgb_value = img->get_RGB(x, y);

				// R hist
				{
					int bin = rgb_value.R / binsize;
					histdata_R[bin]++;
				}
				// G hist
				{
					int bin = rgb_value.G / binsize;
					histdata_G[bin]++;
				}
				// B hist
				{
					int bin = rgb_value.B / binsize;
					histdata_B[bin]++;
				}
			}
		}

		return HistogramRGB{histdata_R, histdata_G, histdata_B};
	}
}
