#include "Plane.h"

Plane::Plane(const plane_t &plane) {
	mPlane = plane;
	type = PLANE;
};

bool Plane::intersect(const ray_t &ray, Intersection_t &hit) {
	float t = ray.intersectPlane(mPlane);
	if ( t != FLT_MAX ) {
		hit.t = t;
		hit.normal = mPlane.normal;
		hit.mat = mat;
		return true;
	}
	return false;
};