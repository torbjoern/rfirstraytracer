#pragma once
#include "Object.h"

class Plane : Object {
private:
	plane_t mPlane;
	
public:
	virtual bool intersect(ray_t ray);
	Plane(plane_t plane);
	float t;
};
