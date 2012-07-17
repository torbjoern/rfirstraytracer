#include "Plane.h"

Plane::Plane(plane_t plane) {
	mPlane = plane;

	type = PLANE;
};

bool Plane::intersect(ray_t ray) {
	float planeDist = mPlane.position.length();
	t = (planeDist - mPlane.normal.dot(ray.origin)) / mPlane.normal.dot(ray.dir);

	if (t > 0.f)
		return true;
	return false;
};