#pragma once
#include "Object.h"

class Plane : public Object {
private:
	plane_t mPlane;
	
public:
	virtual bool intersect(const ray_t &ray, Intersection_t &hit);
	Plane(plane_t plane);
};
