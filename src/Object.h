#pragma once
#include "vec_math.h"

class Material;// fwd decl

class Object {
public:
	Object() {}
	virtual ~Object() {}
	int type;
	Material *mat;
	virtual bool intersect(const ray_t &ray, Intersection_t &hit) = 0;
};
