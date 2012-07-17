#pragma once
#include "Object.h"

class Plane : public Object {
private:
	plane_t mPlane;
	
public:
	virtual bool intersect(ray_t ray);
	Plane(plane_t plane);
};
