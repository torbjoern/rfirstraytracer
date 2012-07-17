#include "Monkey.h"
#include "monkeydata.h"

Monkey::Monkey()
{
	verts.clear();

	int stride = 3*2;
	for (int i=0; i<(3*NUM_FACES); i+=3) {
		// Read indices for a face (a,b,c)
		int ia = stride*monkeyf[i+0];
		int ib = stride*monkeyf[i+1];
		int ic = stride*monkeyf[i+2];

		// Flip Y-axis
		const vec3 va = vec3(monkey_verts[ia+0], monkey_verts[ia+1], monkey_verts[ia+2]);
		const vec3 vb = vec3(monkey_verts[ib+0], monkey_verts[ib+1], monkey_verts[ib+2]);
		const vec3 vc = vec3(monkey_verts[ic+0], monkey_verts[ic+1], monkey_verts[ic+2]);

		verts.push_back( va );
		verts.push_back( vb );
		verts.push_back( vc );
	}
	// Finally, calc bounding box min,max for vertices
	calcBounds();

}
