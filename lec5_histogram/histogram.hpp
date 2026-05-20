#include "myImageIO.h"
#include <variant>

struct HistogramRGB {
	int* hist_data_R;
	int* hist_data_G;
	int* hist_data_B;
};

int* Histogram(myImageData* img, int binsize, bool print_histogram = true);

std::variant<int*, HistogramRGB> Histogram_color(myImageData* img, int binsize,
												 bool luminance_only = true);