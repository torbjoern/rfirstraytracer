
#pragma once;
#include <vector>

struct Pixel_t
{
	unsigned char r,g,b;
};

// fwd decl
struct Camera_t;
void trace( std::vector<std::vector<Pixel_t>> &pixels, int width, int height, const Camera_t &camera );