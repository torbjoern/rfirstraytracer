#include "trimesh.h"

bool Trimesh::intersect(const ray_t &ray, Intersection_t &hit)
{
	float boxnear, boxfar;
	if ( !ray.CheckBoxIntersection(bounds_min, bounds_max, boxnear, boxfar ) )
	{
		return false;
	}

	const size_t numVerts = verts.size();
	int num_tris_hit = 0;
	for ( size_t i=0; i<numVerts; i+=3 ) {
		const vec3 &a = verts[i+0];
		const vec3 &b = verts[i+1];
		const vec3 &c = verts[i+2];

		vec3 geometric_normal;
		const float t_triangle = ray.intersectTriangle( a,b,c, geometric_normal );
		if ( t_triangle < FLT_MAX ) {
			num_tris_hit++;
			hit.t = t_triangle;
			const vec3 hitPoint = ray.origin + hit.t * ray.dir;
			hit.normal = geometric_normal; // TODO, interpolate normals with barycentric coords (uvw)
		}
	}
	return num_tris_hit > 0;
}