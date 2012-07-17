#include "raytracer.h"
#include "vec_math.h"
#include "trimesh.h"

#include <iostream>

struct Camera_t
{
	vec3 position;
	vec3 lookAt;
	vec3 lookUp;
};

struct Light_t
{
};

Pixel_t vec2color(const vec3 &v)
{
	Pixel_t c = { byte(255.f*v.x), byte(255.f*v.y), byte(255.f*v.z) };
	return c;
}

/*
takes a vector of pixels - the image to write to

creates a camera basis, a scene, traces the scene.
*/
void trace( std::vector<std::vector<Pixel_t>> &pixels, int width, int height )
{
	const Pixel_t red = {255,0,0};
	const Pixel_t black = {0,0,0};
	const Pixel_t white = {255,255,255};
	const Pixel_t blue = {0,0,255};
	
	// Vertical and horizontal Field of View
	float hfov = float(M_PI) / 3.5f;
	float vfov = hfov * height/float(width);

	// Pixel width and height
	float pw = 2.0f * tan( hfov/2.0f ) / float(width);
	float ph = 2.0f * tan( vfov/2.0f ) / float(height);

	// Create easy camera definition, and the basis vectors (u,v,n)
	Camera_t camera;
	camera.position = vec3(0.f, 0.f, -4.f);
	camera.lookAt =   vec3(0.f, 0.f, 1.f);
	camera.lookUp =   vec3(0.f, 1.f, 0.f);

	// Setup basis coordinate system
	vec3 n = camera.position - camera.lookAt;
	vec3 u = camera.lookUp.cross(n);
	vec3 v = n.cross(u);

	u.normalize();
	v.normalize();
	n.normalize();

	vec3 vpc = camera.position - n; // viewport center

	plane_t plane( vec3(0.f, -5.f,0.f), vec3(0.f, 1.f, 0.f) );

	Trimesh trimesh;
	trimesh.createMonkey();

	for ( int x=0; x<width; x++ ) {
	for ( int y=0; y<height; y++ ) {
	
		const vec3 uScaled = u * (x-width/2.f)*pw;
		const vec3 vScaled = v * (y-height/2.f)*ph;

		const vec3 rayPoint = vpc + uScaled + vScaled;
		const vec3 rayDir = (rayPoint - camera.position).normalize();

		int material_index = -1;
		const ray_t ray( rayPoint, rayDir );
		float tmin = FLT_MAX;
		vec3 shadingNormal;
		
		const vec3 sphereCenter1 = trimesh.bounds_min;
		const float tSphere1 = ray.intersectSphere( sphereCenter1, 0.2f );
		if ( tSphere1 < tmin ) {
			tmin = tSphere1;
			material_index = 0;
			const vec3 intersection = ray.origin + tmin * ray.dir;
			shadingNormal = (intersection-sphereCenter1).normalize();
		}

		const vec3 sphereCenter2 = trimesh.bounds_max;
		const float tSphere2 = ray.intersectSphere( sphereCenter2, 0.2f );
		if ( tSphere2 < tmin ) {
			tmin = tSphere2;
			material_index = 0;
			const vec3 intersection = ray.origin + tmin * ray.dir;
			shadingNormal = (intersection-sphereCenter2).normalize();
		}

		const float tPlane = ray.intersectPlane( plane );
		if ( tPlane < tmin ) {
			tmin = tPlane;
			material_index = 2;
			shadingNormal = plane.normal;
		}

		//float boxin, boxout;
		//bool hitBox = ray.CheckBoxIntersection( vec3(-0.5f,-0.5f,-0.5f), vec3(+0.5f,+0.5f,+0.5f), boxin, boxout);
		//if ( hitBox && boxin < tmin ) {
		//	tmin = boxin;
		//	material_index = 3;
		//	//shadingNormal = plane.normal;
		//}

		//const float tTri = ray.intersectTriangle( vec3(-.5f,-.5f,0.f), vec3(+.5f,-.5f,0.f), vec3(0.f,+0.5f,0.f) );
		
		Intersection_t isect = trimesh.trace( ray );
		if ( isect.t < tmin ) {
			tmin = isect.t;
			material_index = 0;
			shadingNormal = isect.normal;
		}

		const vec3 intersection = ray.origin + tmin * ray.dir;

		Pixel_t pixelColor = white;
		if ( material_index == 0 ) { pixelColor = vec2color(.5f + .5f*shadingNormal); }
		if ( material_index == 1 ) { pixelColor = vec2color( (ray.origin + ray.dir * tmin).normalize() ); }
		if ( material_index == 2 ) {
			float scale = 0.5f;
			int sum = int(scale*intersection.x) + int(scale*intersection.y) + int(scale*intersection.z);
			bool flip = pow(-1.0, sum) > 0.0;
			if ( flip ) { pixelColor = red; } else { pixelColor = black; }
		}
		if ( material_index == 3 ) {
			pixelColor = blue;
		}

		pixels[x][y] = pixelColor;
	}
		std::cout << "x=" << x << std::endl;
	}
}

void shade()
{
}