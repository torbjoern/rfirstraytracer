#pragma once
#include "Object.h"

class Sphere : public Object {
private:
	plane_t mPlane;
	vec3 position;
	float radius;
	
public:
	virtual bool intersect(ray_t ray);
	Sphere(vec3 pos, float radi, float tmin);
};
