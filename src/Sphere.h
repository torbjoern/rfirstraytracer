#pragma once
#include "Object.h"

class Sphere : public Object {
private:
	plane_t mPlane;
	vec3 position;
	float radius;
	
public:
	virtual bool intersect(const ray_t &ray, Intersection_t &hit);
	Sphere(vec3 pos, float radi, float tmin);
};
