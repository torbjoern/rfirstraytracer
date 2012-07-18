
#pragma once;
#include <vector>

struct Pixel_t
{
	unsigned char r,g,b;
};

const Pixel_t red = {255,0,0};
const Pixel_t black = {0,0,0};
const Pixel_t white = {255,255,255};
const Pixel_t blue = {0,0,255};
const Pixel_t cls_color = {51,171,249};

static const int MAX_RECURSIVE_TRACES = 3;

// fwd decl
struct Camera_t;
class Scene;

void trace( const Scene &scene, const Camera_t &camera, std::vector<std::vector<Pixel_t>> &pixels, int width, int height );