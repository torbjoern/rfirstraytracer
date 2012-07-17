#pragma once;

#include "vec_math.h"

#include "Object.h"
#include <vector>

class Trimesh : public Object
{
public:
	Trimesh() {} // Dummy for derived classes...
	Trimesh(const std::vector<vec3> &verts);
	virtual bool intersect(const ray_t &ray, Intersection_t &hit);
	vec3 bounds_min;
	vec3 bounds_max;

protected:
	void calcBounds();
protected:
	std::vector<vec3> verts;
};