#include "raytracer.h"
#include "vec_math.h"
#include "trimesh.h"
#include "Object.h"
#include "Plane.h"
#include "Sphere.h"
#include "Monkey.h"
#include "Material.h"

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
	const Pixel_t cls_color = {51,171,249};
	
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


	typedef std::vector<Object*> SceneList;
	SceneList mainScene;

	MatChequered sjakkMat( vec3(1.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f) );
	MatNormal normalMat;
	MatPhong phongMat;

	// Add objects to the scene
	Sphere *sphere = new Sphere( vec3(1.f,1.f,1.f), 0.4f );
	sphere->mat = &normalMat;

	Plane *plane = new Plane( plane_t (vec3(0.f, -5.f,0.f), vec3(0.f, 1.f, 0.f)));
	plane->mat = &sjakkMat;

	Monkey *monkey = new Monkey;
	monkey->mat = &phongMat;

	mainScene.push_back( sphere );
	mainScene.push_back( plane );
	mainScene.push_back( monkey );

	
	
	// For every pixel
	for ( int x=0; x<width; x++ ) {
		for ( int y=0; y<height; y++ ) {
	
			const vec3 uScaled = u * (x-width/2.f)*pw;
			const vec3 vScaled = v * (y-height/2.f)*ph;

			// Construct a ray from the eye 
			const vec3 rayPoint = vpc + uScaled + vScaled;
			const vec3 rayDir = (rayPoint - camera.position).normalize();

			const ray_t ray( rayPoint, rayDir );

			// For every object in the scene
			Intersection_t closestHit;
			closestHit.t = FLT_MAX;

			Intersection_t currentHit;
		
			for ( size_t i=0; i < mainScene.size(); i++) {
				// Find intersection with the ray
				if (mainScene[i]->intersect( ray, currentHit ) ) {
					// Keep if closest
					if ( currentHit.t < closestHit.t ) 
					{ 
						closestHit = currentHit; 
						closestHit.mat = mainScene[i]->mat; // Copy pointer to material
					}
				}
			}

			Pixel_t pixelColor = cls_color;
			if ( closestHit.t != FLT_MAX ) 
			{
				const vec3 hitPoint = ray.origin + closestHit.t * ray.dir;
				vec3 shaded = closestHit.mat->shade( hitPoint, closestHit.normal );
				pixelColor = vec2color(shaded);
			}
			pixels[x][y] = pixelColor;
		} // for y

		printf("x = %d\n", x);
	} //for x
}
