#include "Plane.h"

Plane::Plane(plane_t plane) {
	mPlane = plane;

	type = PLANE;
};

bool Plane::intersect(const ray_t &ray, Intersection_t &hit) {
	float planeDist = mPlane.position.length();
	hit.t = (planeDist - mPlane.normal.dot(ray.origin)) / mPlane.normal.dot(ray.dir);
	
	if (hit.t > 0.f) {
		hit.normal = mPlane.normal;
		return true;
	}
	return false;
};