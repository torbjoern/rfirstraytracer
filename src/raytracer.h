
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

class Object;
typedef std::vector<Object*> SceneList;

// fwd decl
struct Camera_t;
SceneList createScene();
//void castRay( const ray_t &ray, const SceneList &mainScene,  Intersection_t &closestHit );
void trace( std::vector<std::vector<Pixel_t>> &pixels, int width, int height, const Camera_t &camera, const SceneList &mainScene );