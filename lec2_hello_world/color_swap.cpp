#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include "myImageIO.h"

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

	// prepare img2

	myImageData *img2 = new myImageData();
	img2->init(W, H, CH);

	// processing

	for (int y = 0; y < H; y++)
	{
		for (int x = 0; x < W; x++)
		{
			RGB rgb_value = img1->get_RGB(x,y);
            RGB new_rgb{};
            new_rgb.R = rgb_value.G;
            new_rgb.G = rgb_value.B;
            new_rgb.B = rgb_value.R;
            img2->set(x, y, new_rgb);
		}
	}

	std::cout << argv[1] << std::endl;

	std::string output_name = "color_swap";

	const char *output_name_char = output_name.c_str();
	img2->save(output_name_char);

	delete img1;
	delete img2;

	return 0; // normal termination
}