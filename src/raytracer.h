
#pragma once;
#include <vector>

struct Pixel_t
{
	unsigned char r,g,b;
};

void trace( std::vector<std::vector<Pixel_t>> &pixels, int width, int height );