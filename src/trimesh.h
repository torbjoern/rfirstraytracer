#include "vec_math.h"

#include <vector>

class Trimesh
{
public:
	void createMonkey();
	Intersection_t trace(const ray_t &ray);

	vec3 bounds_min;
	vec3 bounds_max;
private:
	std::vector<vec3> verts;

};