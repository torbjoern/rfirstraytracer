#pragma once
#include "vec_math.h"

class Object {
public:
	Object() {}
	virtual ~Object() {}
	float t;
	int type;
	virtual bool intersect(const ray_t &ray, Intersection_t &hit) = 0;
};
