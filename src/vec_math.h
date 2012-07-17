#pragma once
#define _USE_MATH_DEFINES
#include <math.h>


struct vec3
{
	float x,y,z;

	vec3(){ x=0; y=0; z=0;} // default
	vec3(float x, float y, float z) : x(x), y(y), z(z) {} // just set xyz

	float dot(const vec3 b) const
	{
		const vec3 &a = *this; 
		return a.x*b.x + a.y*b.y + a.z*b.z;
	}

	vec3 cross(const vec3 &b) const
	{
		const vec3 &a = *this; 
		return vec3(
		 a.y * b.z - a.z * b.y,
		 a.z * b.x - a.x * b.z,
		 a.x * b.y - a.y * b.x
		 );
	}

	float length() const
	{
		return sqrt(x*x + y*y + z*z);
	}

	vec3 normalize()
	{
		float len = length();
		if ( len ) {
			float rlen = 1.0f / len;
			x *= rlen;
			y *= rlen;
			z *= rlen;
		}
		return *this;
	}
};
// Create binary, and two scalar overloads. This trick taken from cube2 src
#define VEC_OP(OP)                                                    \
static inline vec3 operator OP (const vec3 &a, const vec3 &b) {          \
	return vec3(a.x OP b.x, a.y OP b.y, a.z OP b.z);                   \
}                                                                     \
static inline vec3 operator OP (float s, const vec3 &b) {               \
	return vec3(s OP b.x, s OP b.y, s OP b.z);                         \
}                                                                     \
static inline vec3 operator OP (const vec3 &a, float s) {               \
	return vec3(a.x OP s, a.y OP s, a.z OP s);                         \
}
VEC_OP(*)
VEC_OP(/)
VEC_OP(+)
VEC_OP(-)
#undef VEC_OP


struct plane_t
{
	vec3 position;
	vec3 normal;

	plane_t() {};
	plane_t(vec3 position, vec3 normal) : position(position), normal(normal) {}
};

enum object_type {
	SPHERE,
	PLANE
};

struct ray_t
{
	vec3 origin;
	vec3 dir;

	ray_t(vec3 origin, vec3 dir) : origin(origin), dir(dir) {}

	bool intersectSphere( vec3 center, float radi, float &t ) const
	{
		vec3 m = origin - center;
		float b = m.dot(dir);
		float c = m.dot(m) - radi*radi;
		// exit if r's origin is outside s (c>0) and r pointing away from s (b>0)
		if ( c > 0.0 && b > 0.0 ) return false;
		float discr = b*b - c;
		if ( discr < 0.0f ) return 0.f;
		t = -b - sqrt(discr);
		if ( t < 0.0f ) t = 0.0f; // clamp when inside sphere
		return true;
	}

	// doesn't calculate t, only tests if we hit or not
	bool intersectSphereTest( vec3 center, float radi ) const
	{
		vec3 s = this->origin - center;
		float sv = s.dot(this->dir);
		float ss = s.dot(s);
		float discr = sv*sv - ss + radi*radi;
		if ( discr < 0.0f ) return false;
		return true;
	}

	float intersectPlane( const plane_t &p ) const
	{
		const ray_t &r = *this;
		float planeDist = p.position.length();
		float t = (planeDist - p.normal.dot(r.origin)) / p.normal.dot(r.dir);
		return t;
	}
};