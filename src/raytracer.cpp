#include "raytracer.h"
#include "vec_math.h"
#include "trimesh.h"
#include "Object.h"
#include "Plane.h"
#include "Sphere.h"
#include "Monkey.h"
#include "Material.h"

#include <iostream>


Pixel_t vec2color(const vec3 &v)
{
	Pixel_t c = { byte(255.f*v.x), byte(255.f*v.y), byte(255.f*v.z) };
	return c;
}

void castRay( const ray_t &ray, std::vector<Object*> &mainScene,  Intersection_t &closestHit )
{
	for ( size_t i=0; i < mainScene.size(); i++) {
		// Find intersection with the ray
		Intersection_t currentHit;
		if (mainScene[i]->intersect( ray, currentHit ) ) {
			// Keep if closest
			if ( currentHit.t < closestHit.t ) 
			{ 
				closestHit = currentHit; 
				closestHit.mat = mainScene[i]->mat; // Copy pointer to material
			}
		}
	}
}

/*
takes a vector of pixels - the image to write to

creates a camera basis, a scene, traces the scene.
*/
void trace( std::vector<std::vector<Pixel_t>> &pixels, int width, int height, const Camera_t &camera )
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

	// Setup basis coordinate system
	vec3 n = normalize( camera.position - camera.lookAt ); // from lookat to pos???
	vec3 u = normalize( cross(camera.lookUp,n) );
	vec3 v = normalize( cross(n,u) );

	vec3 vpc = (camera.position - n); // viewport center

	vec3 lightPos = vec3(-10.f, -10.f, 0.f);

	typedef std::vector<Object*> SceneList;
	SceneList mainScene;

	MatChequered sjakkMat( vec3(1.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f) );
	MatChequered blackWhiteChequerMat( vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f), 5.f );
	MatNormal normalMat;
	MatPhong phongMat( vec3(0.7f, 0.f, 0.f) );

	// Add objects to the scene
	Sphere *sphere = new Sphere( vec3(1.f,-1.f,1.f), 0.4f );
	sphere->mat = &phongMat;

	Plane *plane = new Plane( plane_t (vec3(0.f,-1.f,0.f), vec3(0.f, 1.f, 0.f)));
	plane->mat = &sjakkMat;

	//Monkey *monkey = new Monkey;
	//monkey->mat = &phongMat;

	mainScene.push_back( sphere );
	mainScene.push_back( plane );
	//mainScene.push_back( monkey );

	std::vector<Material*> materials;

	int pointsInCircle = 10;
	for ( int i=0; i<pointsInCircle; i++ ){
		float a = 2.f*(float)M_PI * i/float(pointsInCircle);
		float radi = 2.f;
		Sphere *sphere = new Sphere( vec3(cos(a)*radi,-0.4f,sin(a)*radi), 0.4f );
		Material* sphereMat = new MatPhong( vec3( .5f+.5f*cos(a), 0.f, .5f+.5f*sin(a) ).normalize() );
		materials.push_back( sphereMat );
		sphere->mat = sphereMat;
		mainScene.push_back( sphere );
	}

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

			castRay( ray, mainScene, closestHit );

			Pixel_t pixelColor = cls_color;
			if ( closestHit.t != FLT_MAX ) 
			{
				const vec3 hitPoint = ray.origin + closestHit.t * ray.dir;

				Intersection_t shadowHit;
				shadowHit.t = FLT_MAX;
				vec3 toLight = (lightPos - hitPoint).normalize();
				const ray_t shadowRay(hitPoint, toLight );
				castRay( shadowRay, mainScene, shadowHit );
				float shadowAtt = 1.f;
				if ( shadowHit.t == FLT_MAX ) {
					// Nothing hit towards light! No attenuation
				} else {
					shadowAtt = 0.4f;
				}

				vec3 shaded = shadowAtt * closestHit.mat->shade( hitPoint, closestHit.normal );
				pixelColor = vec2color(shaded);
			}
			pixels[x][y] = pixelColor;
		} // for y

		printf("x = %d\n", x);
	} //for x
}
