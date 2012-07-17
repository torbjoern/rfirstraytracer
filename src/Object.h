#pragma once
#include "vec_math.h"

class Object {
public:
	Object();
	float t;
	virtual bool intersect(ray_t ray);
};
