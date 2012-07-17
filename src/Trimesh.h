#pragma once;

#include "vec_math.h"

#include "Object.h"
#include <vector>

class Trimesh : public Object
{
public:
	virtual bool intersect(const ray_t &ray, Intersection_t &hit);
	vec3 bounds_min;
	vec3 bounds_max;
protected:
	std::vector<vec3> verts;

};