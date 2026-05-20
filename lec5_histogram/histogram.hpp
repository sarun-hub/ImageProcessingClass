#include "myImageIO.h"
#include <variant>

enum HistMode { Grey = 99, Luminance = -1, Red = 0, Green = 1, Blue = 2 };

int* Histogram(myImageData* img, int binsize, HistMode mode = HistMode::Grey,
			   bool print_histogram = true);