#include "raytracer.h"
#include "vec_math.h"

struct Camera_t
{
	vec3 position;
	vec3 lookAt;
	vec3 lookUp;
};

Pixel_t vec2color(const vec3 &v)
{
	Pixel_t c = {255.f*v.x, 255.f*v.y, 255.f*v.z};
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
	
	// Vertical and horizontal Field of View
	float hfov = float(M_PI) / 3.5f;
	float vfov = hfov * height/float(width);

	// Pixel width and height
	float pw = 2.0f * tan( hfov/2.0f ) / float(width);
	float ph = 2.0f * tan( vfov/2.0f ) / float(height);

	// Create easy camera definition, and the basis vectors (u,v,n)
	Camera_t camera;
	camera.position = vec3(0.f, 0.f, 0.f);
	camera.lookAt =   vec3(1.f, 1.f, 1.f);
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

	for ( int x=0; x<width; x++ ) {
	for ( int y=0; y<height; y++ ) {
	
		const vec3 uScaled = u * (x-width/2.f)*pw;
		const vec3 vScaled = v * (y-height/2.f)*ph;

		const vec3 rayPoint = vpc + uScaled + vScaled;
		const vec3 rayDir = (rayPoint - camera.position).normalize();

		const ray_t ray( rayPoint, rayDir );
		float tSphere = 0.f;
		const bool hitSphere = ray.intersectSphere( vec3(1.f,1.f,1.f), 0.4, tSphere);
		const float tPlane = ray.intersectPlane( plane );
		const bool hitPlane = tPlane > 0.f;		

		Pixel_t pixelColor = white;
		if ( hitSphere || hitPlane ) {
			float tmin = tSphere;

			if ( hitSphere && hitPlane ) {
				if ( tPlane < tSphere ) {
					tmin = tPlane;
					pixelColor = vec2color( (ray.origin + ray.dir * tmin).normalize() );
				} else {
					tmin = tSphere,
					pixelColor = red;
				}
			} else if (hitSphere) {
				tmin = tSphere;
				pixelColor = red;
			} else if (hitPlane) {
				tmin = tPlane;
				pixelColor = vec2color( vec3(tmin, tmin, tmin) );
			}

		} 
		pixels[x][y] = pixelColor;
	}}
}