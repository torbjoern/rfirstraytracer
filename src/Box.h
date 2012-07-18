#include "Object.h"

class Box : public Object
{
public:
	Box( const vec3 &boxMin, const vec3 &boxMax ) : boxMin(boxMin), boxMax(boxMax)
	{
	}

	virtual bool intersect(const ray_t &ray, Intersection_t &hit);
private:
	vec3 boxMin;
	vec3 boxMax;
};