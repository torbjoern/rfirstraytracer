#include "Sphere.h"

Sphere::Sphere(vec3 pos, float radi) {
	position = pos;
	radius = radi;

	type = SPHERE;
};

bool Sphere::intersect(const ray_t &ray, Intersection_t &hit) 
{
	float t = ray.intersectSphere(position, radius, hit.isInside);
	if ( t!=FLT_MAX){
		hit.t = t;
		hit.normal = normalize( (ray.origin + hit.t*ray.dir) - position);
		hit.mat = mat;
		return true;
	}
	return false;
};