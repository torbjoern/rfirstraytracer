#include "trimesh.h"

#include "monkey.h"

void Trimesh::createMonkey()
{
	verts.clear();

	bounds_min = vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	bounds_max = vec3(FLT_MIN, FLT_MIN, FLT_MIN);

	int stride = 3*2;
	for (int i=0; i<(3*NUM_FACES); i+=3) {
		// Read indices for a face (a,b,c)
		int ia = stride*monkeyf[i+0];
		int ib = stride*monkeyf[i+1];
		int ic = stride*monkeyf[i+2];

		// Flip Y-axis
		const vec3 va = vec3(monkey_verts[ia+0], -monkey_verts[ia+1], monkey_verts[ia+2]);
		const vec3 vb = vec3(monkey_verts[ib+0], -monkey_verts[ib+1], monkey_verts[ib+2]);
		const vec3 vc = vec3(monkey_verts[ic+0], -monkey_verts[ic+1], monkey_verts[ic+2]);

		verts.push_back( va );
		verts.push_back( vb );
		verts.push_back( vc );
		
		for ( int dim=0; dim<3; dim++){
			if ( va[dim] < bounds_min[dim] ) bounds_min[dim] = va[dim];
			if ( vb[dim] < bounds_min[dim] ) bounds_min[dim] = vb[dim];
			if ( vc[dim] < bounds_min[dim] ) bounds_min[dim] = vc[dim];

			if ( va[dim] > bounds_max[dim] ) bounds_max[dim] = va[dim];
			if ( vb[dim] > bounds_max[dim] ) bounds_max[dim] = vb[dim];
			if ( vc[dim] > bounds_max[dim] ) bounds_max[dim] = vc[dim];
		}

	}
}


Intersection_t Trimesh::trace(const ray_t &ray)
{
	Intersection_t isect;
	isect.t = FLT_MAX;

	float boxnear, boxfar;
	if ( !ray.CheckBoxIntersection(bounds_min, bounds_max, boxnear, boxfar ) )
	{
		return isect;
	}

	const size_t numVerts = verts.size();
	for ( size_t i=0; i<numVerts; i+=3 ) {
		const vec3 &a = verts[i+0];
		const vec3 &b = verts[i+1];
		const vec3 &c = verts[i+2];

		const float tTri = ray.intersectTriangle( a,b,c );
		if ( tTri < isect.t  ) {
			isect.t = tTri;
			const vec3 hitPoint = ray.origin + isect.t * ray.dir;
			isect.normal = (hitPoint - a).normalize();
		}
	}
	
	return isect;
}