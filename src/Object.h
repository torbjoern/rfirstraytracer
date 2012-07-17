#pragma once
#include "vec_math.h"

class Object {
public:
	Object();
	float t;
	int type;
	virtual bool intersect(ray_t ray) = 0;
};
