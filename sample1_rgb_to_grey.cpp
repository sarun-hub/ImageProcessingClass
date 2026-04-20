#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include "myImageIO.h"
#include <vector>

int main(int argc, char **argv)
{

	// read image data to img1

	myImageData *img1 = new myImageData();
	img1->read(argv[1]);

	int W = img1->getWidth();
	int H = img1->getHeight();
	int CH = img1->getCH();

	printf("resolution: %d x %d\n", W, H);
	printf("channel: %d\n", CH);

	std::vector<double> luminous_weight = {0.299, 0.587, 0.114};

	// prepare img2

	myImageData *img2 = new myImageData();
	// get rgb (3 channels) to grey scale (1 channel)
	img2->init(W, H, 1);

	// processing

	for (int y = 0; y < H; y++)
	{
		for (int x = 0; x < W; x++)
		{

			double grey_scale_value = 0;
			for (int channel = 0; channel < CH; channel++)
			{

				double value = img1->get(x, y, channel);
				grey_scale_value += luminous_weight[channel] * value;
			}
			img2->set(x, y, 0, static_cast<int>(grey_scale_value));
		}
	}

	std::string output_name = "result_rgb_to_grey";
	// if (CH == 3)
	// {
	// 	output_name = "colored_inverted";
	// }

	const char *output_name_char = output_name.c_str();
	img2->save(output_name_char);

	delete img1;
	delete img2;

	return 0; // normal termination
}