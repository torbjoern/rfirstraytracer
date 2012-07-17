#include "Sphere.h"

Sphere::Sphere(vec3 pos, float radi, float tmin) {
	position = pos;
	radius = radi;
	t = tmin;
};

bool Sphere::intersect(ray_t ray) {
	vec3 m = ray.origin - position;
	float b = m.dot(ray.dir);
	float c = m.dot(m) - radius*radius;
	// exit if r's origin is outside s (c>0) and r pointing away from s (b>0)
	if ( c > 0.0 && b > 0.0 ) return false;
	float discr = b*b - c;
	if ( discr < 0.0f ) return 0.f;
	t = -b - sqrt(discr);
	if ( t < 0.0f ) t = 0.0f; // clamp when inside sphere
	return true;
};