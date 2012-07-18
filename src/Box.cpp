#include "Box.h"

bool Box::intersect(const ray_t &ray, Intersection_t &hit)
{
	float tnear, tfar;
	if ( ray.CheckBoxIntersection( boxMin, boxMax, tnear, tfar) )
	{
		
		hit.t = tnear;
		hit.normal = (ray.origin + tnear * ray.dir).normalize();
		return true;
	}

	return false;
}