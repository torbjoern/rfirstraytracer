#include "raytracer.h"
#include "vec_math.h"
#include "trimesh.h"
#include "Object.h"
#include "Plane.h"
#include "Sphere.h"
#include "Box.h"
#include "Monkey.h"
#include "Material.h"

#include <iostream>

Pixel_t vec2color(const vec3 &v)
{
	Pixel_t c = { byte(255.9f*v.x), byte(255.9f*v.y), byte(255.9f*v.z) };
	if ( c.r > 255 ) c.r = 255;
	if ( c.g > 255 ) c.g = 255;
	if ( c.b > 255 ) c.b = 255;
	return c;
}

void castRay( const ray_t &ray, const std::vector<Object*> &mainScene,  Intersection_t &closestHit )
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

bool castShadowRay( const ray_t &ray, const std::vector<Object*> &mainScene )
{
	for ( size_t i=0; i < mainScene.size(); i++) {
		// Find intersection with the ray
		Intersection_t currentHit;
		if (mainScene[i]->intersect( ray, currentHit ) ) {
			return true;
		}
	}
	return false;
}

SceneList createScene()
{
	SceneList mainScene;
	std::vector<Material*> materials;
	Material* sjakkMat = new MatChequered( vec3(1.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f) );
	Material* blackWhiteChequerMat = new MatChequered( vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f), 5.f );
	Material* normalMat = new MatNormal;
	Material* phongMat = new MatPhong( vec3(0.9f, 0.4f, 0.1f).normalize() );
	
	Material* reflPhongMat = new MatPhong( vec3(0.8f, 0.8f, 0.8f).normalize() );
	reflPhongMat->isReflective = true;

	materials.push_back(sjakkMat);
	materials.push_back(blackWhiteChequerMat);
	materials.push_back(normalMat);
	materials.push_back(phongMat);
	materials.push_back(reflPhongMat);

	// Add objects to the scene

	Plane *plane = new Plane( plane_t (vec3(0.f,1.1f,0.f), vec3(0.f, 1.f, 0.f))); // TODO. get epsilon working at origin
	plane->mat = sjakkMat;
	mainScene.push_back( plane );

	vec3 boxPos( 2.f, 0.f, 0.f );
	Box *box = new Box( boxPos+vec3(-.5f, -.5f, -.5f), boxPos+vec3(.5f, .5f, .5f) );
	box->mat = normalMat;
	mainScene.push_back( box );

	Sphere *sphere = new Sphere( vec3(0.f, 0.f,0.f), 0.8f );
	sphere->mat = reflPhongMat;
	mainScene.push_back( sphere );

	//Monkey *monkey = new Monkey;
	//monkey->mat = reflPhongMat;
	//mainScene.push_back( monkey );
	
	//std::vector<vec3> tris;
	//tris.push_back( vec3(-.5f,-.5f,0.f) );
	//tris.push_back( vec3(+.5f,-.5f,0.f) );
	//tris.push_back( vec3(0.f,+0.5f,0.f) );
	//Trimesh* singleTriangle = new Trimesh( tris );
	//singleTriangle->mat = phongMat;
	//mainScene.push_back( singleTriangle );

	int pointsInCircle = 8;
	for ( int i=0; i<pointsInCircle; i++ ){
		float a = 2.f*(float)M_PI * i/float(pointsInCircle);
		float radi = 3.f;
		const vec3 pos(cos(a)*radi,0.0f,sin(a)*radi);
		Sphere *sphere = new Sphere( pos, 0.4f );
		Material* sphereMat = new MatPhong( vec3( .5f+.5f*cos(a), 0.f, .5f+.5f*sin(a) ).normalize() );
		//sphereMat->isReflective = false;
		materials.push_back( sphereMat );
		sphere->mat = sphereMat;
		mainScene.push_back( sphere );
	}

	

	//int pointsInLine = 20.f;
	//for ( int i=0; i<pointsInLine; i++ ){
	//	float pos =  i/float(pointsInLine);
	//	Sphere *sphere = new Sphere( vec3(-3.0f + 3.f*pos,1.32f,0.f), 0.32f );
	//	sphere->mat = phongMat;
	//	mainScene.push_back( sphere );
	//}

	return mainScene;
}

vec3 shade( const SceneList &mainScene,
			const Intersection_t &closestHit,
			const vec3 &hitPoint,
			const ray_t &ray,
			const std::vector<vec3> &light_positions, float shadowContrib)
{
	float shadowAtt = 1.f;

	size_t num_lights = light_positions.size();
	for (size_t iLight=0; iLight<num_lights; iLight++){
		const vec3 &lightPos = light_positions[iLight];
		vec3 toLight = (lightPos - hitPoint).normalize();
		const ray_t shadowRay(hitPoint+epsilon*toLight, toLight );
		if ( castShadowRay( shadowRay, mainScene ) ) {
			shadowAtt -= shadowContrib;
		}
	}

	vec3 shaded(0.f,0.f,0.f);
	for (size_t iLight=0; iLight<num_lights; iLight++){
		const vec3 &lightPos = light_positions[iLight];
		shaded = shaded + closestHit.mat->shade( lightPos, hitPoint, closestHit.normal, ray );
	}
	return shadowAtt*shaded / num_lights;
	
}

void raytrace( const SceneList &mainScene, 
	           const std::vector<vec3> &light_positions, 
			   float shadowContrib,
			   const ray_t &ray, int recursionDepth, vec3 &acc )
{
	if ( recursionDepth > MAX_RECURSIVE_TRACES ) {
		return;
	}

	// For every object in the scene
	Intersection_t closestHit;
	closestHit.t = FLT_MAX;

	castRay( ray, mainScene, closestHit );

	if ( closestHit.t != FLT_MAX ) 
	{
		const vec3 hitPoint = ray.origin + closestHit.t * ray.dir;
		const vec3 diff = shade(mainScene, closestHit, hitPoint, ray, light_positions, shadowContrib );
		acc = acc + diff;

		// Reflection
		if ( closestHit.mat->isReflective && recursionDepth < MAX_RECURSIVE_TRACES ) 
		{
			const vec3 &N = closestHit.normal;
			vec3 R = ray.dir - 2.0f * ray.dir.dot(N) * N;
			vec3 rcol(0.f,0.f,0.f);
			raytrace(mainScene, light_positions, shadowContrib, ray_t(hitPoint+R*epsilon, R), recursionDepth+1, rcol);
			acc = acc + 0.3f * rcol;
		}		
	} else {
		acc = acc + vec3(51/255.f,171/255.f,249/255.f);
	}
}

/*
takes a vector of pixels - the image to write to

creates a camera basis, a scene, traces the scene.
*/
void trace( std::vector<std::vector<Pixel_t>> &pixels, int width, int height, const Camera_t &camera, const SceneList &mainScene )
{
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

	std::vector<vec3> light_positions;
	light_positions.push_back( vec3(4.f, 4.f, 4.f) );
	light_positions.push_back( vec3(-4.f, 4.f, -4.f) );

	size_t num_lights = light_positions.size();
	float shadowContrib = 0.5f / num_lights;

	// For every pixel
#pragma omp parallel for
	for ( int x=0; x<width; x++ ) {
		for ( int y=0; y<height; y++ ) {

			vec3 final(0.f,0.f,0.f);
#define ANTI_ALIAS
#ifndef ANTI_ALIAS
			const vec3 uScaled = u * (x-width/2.f)*pw;
			const vec3 vScaled = v * ( (height-y)-height/2.f)*ph;

			// Construct a ray from the eye 
			const vec3 rayPoint = vpc + uScaled + vScaled;
			const vec3 rayDir = (rayPoint - camera.position).normalize();

			const ray_t ray( rayPoint, rayDir );
			raytrace( mainScene, light_positions, shadowContrib, ray, 1, final );
#else
			for ( int tx = -1; tx < 2; tx++ ) 
			for ( int ty = -1; ty < 2; ty++ )
			{
				const vec3 uScaled = u * ((tx/2.f+x)-width/2.f)*pw;
				const vec3 vScaled = v * ( (height-(ty/2.f+y) )-height/2.f)*ph;

				// Construct a ray from the eye 
				const vec3 rayPoint = vpc + uScaled + vScaled;
				const vec3 rayDir = (rayPoint - camera.position).normalize();
				const ray_t ray( rayPoint, rayDir );
				raytrace( mainScene, light_positions, shadowContrib, ray, 1, final );
			}
			final = final / 9.f;
#endif

			pixels[x][y] = vec2color(final);
		} // for y

		//printf("x = %d\n", x);
	} //for x
}
