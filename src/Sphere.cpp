#include "Sphere.h"

Sphere::Sphere(vec3 pos, float radi, float tmin) {
	position = pos;
	radius = radi;
	t = tmin;

	type = SPHERE;
};

bool Sphere::intersect(const ray_t &ray, Intersection_t &hit) {
	vec3 m = ray.origin - position;
	float b = m.dot(ray.dir);
	float c = m.dot(m) - radius*radius;
	// exit if r's origin is outside s (c>0) and r pointing away from s (b>0)
	if ( c > 0.0 && b > 0.0 ) return false;
	float discr = b*b - c;
	if ( discr < 0.0f ) return 0.f;
	hit.t = -b - sqrt(discr);
	if ( hit.t < 0.0f ) hit.t = 0.0f; // clamp when inside sphere
	hit.normal = normalize( (ray.origin + hit.t*ray.dir) - position);
	return true;
};