#include "raytracer.h"
#include "vec_math.h"
#include "trimesh.h"
#include "Object.h"
#include "Plane.h"
#include "Sphere.h"
#include "Box.h"
#include "Monkey.h"
#include "Material.h"

#include "Scene.h"
#include "Light.h"

#include <iostream>

Pixel_t vec2color(const vec3 &v)
{
	Pixel_t c = { byte(255.9f*v.x), byte(255.9f*v.y), byte(255.9f*v.z) };
	if ( c.r > 255 ) c.r = 255;
	if ( c.g > 255 ) c.g = 255;
	if ( c.b > 255 ) c.b = 255;
	return c;
}

void castRay( const ray_t &ray, const Scene &scene,  Intersection_t &closestHit )
{
	const auto &primtives = scene.primitives;
	for ( size_t i=0; i < primtives.size(); i++) {
		// Find intersection with the ray
		Intersection_t currentHit;
		if (primtives[i]->intersect( ray, currentHit ) ) {
			// Keep if closest
			if ( currentHit.t < closestHit.t ) 
			{ 
				closestHit = currentHit;
			}
		}
	}
}

bool castShadowRay( const ray_t &ray, const Scene &scene, float shadowT )
{
	const auto &primtives = scene.primitives;
	for ( size_t i=0; i < primtives.size(); i++) {
		
		// Find intersection with the ray
		Intersection_t currentHit;
		if (primtives[i]->intersect( ray, currentHit ) ) {
			if ( currentHit.t <= shadowT && currentHit.mat->castsShadow ) return true;
		}
	}
	return false;
}

vec3 shade( const Scene &scene,
			const Intersection_t &closestHit,
			const vec3 &hitPoint,
			const ray_t &ray,
			float shadowContrib)
{
	float shadowAtt = 1.0f;

	const auto &lights = scene.lights;
	size_t num_lights = lights.size();
	for (size_t iLight=0; iLight<num_lights; iLight++){
		const vec3 &lightPos = lights[iLight]->pos;
		vec3 toLight = lightPos - hitPoint;
		const float tdist = toLight.length();
		toLight = toLight * (1.0f / tdist); // normalize

		//Intersection_t isec;
		//isec.t = FLT_MAX;
		//castRay( ray_t(hitPoint+epsilon*toLight, toLight), scene, isec );

		if ( castShadowRay( ray_t(hitPoint+epsilon*toLight, toLight), scene, tdist ) )
		//if ( isec.t < tdist && isec.mat->castsShadow )
		{
			shadowAtt -= shadowContrib;
		}
	}

	vec3 shaded(0.f,0.f,0.f);
	for (size_t iLight=0; iLight<num_lights; iLight++){
		const vec3 &lightPos = lights[iLight]->pos;
		shaded = shaded + closestHit.mat->shade( lightPos, hitPoint, closestHit.normal, ray );
	}
	return shadowAtt*shaded / float(num_lights);
	
}

void raytrace( const Scene &scene, 
			   float shadowContrib,
			   const ray_t &ray, 
			   int recursionDepth, 
			   vec3 &acc,
			   float input_rIndex = 1.f)
{
	if ( recursionDepth > MAX_RECURSIVE_TRACES ) {
		return;
	}

	// For every object in the scene
	Intersection_t closestHit;
	closestHit.t = FLT_MAX;

	// Primary Ray
	castRay( ray, scene, closestHit );

	if ( closestHit.t != FLT_MAX ) 
	{
		// Diffuse shading and shadow ray
		const vec3 hitPoint = ray.origin + closestHit.t * ray.dir;
		const vec3 diff = shade(scene, closestHit, hitPoint, ray, shadowContrib );
		acc = acc + diff;

		// Reflection
		if ( closestHit.mat->isReflective && recursionDepth < MAX_RECURSIVE_TRACES ) 
		{
			const vec3 &N = closestHit.normal;
			vec3 R = ray.dir - 2.0f * ray.dir.dot(N) * N;
			vec3 rcol(0.f,0.f,0.f);
			raytrace(scene, shadowContrib, ray_t(hitPoint+R*epsilon, R), recursionDepth+1, rcol);
				if (rcol.x > 1.f) rcol.x = 1.f;
				if (rcol.y > 1.f) rcol.y = 1.f;
				if (rcol.z > 1.f) rcol.z = 1.f;
			acc = acc + 0.9f * rcol;
		}
		// Refraction
		if ( closestHit.mat->isTransparent && recursionDepth < MAX_RECURSIVE_TRACES ) 
		{
			const float rIndex = 1.3f;
			const float n = input_rIndex / rIndex;
			vec3 N = closestHit.normal;//normDir; // inside or outside
			if (closestHit.isInside) N = -1.f * N;

			const float cosI = -N.dot(ray.dir);
			const float cosT2 = 1.0f - n*n * (1.0f-cosI*cosI);
			if (cosT2 > 0.0f)
			{
				const vec3 T = (n*ray.dir) + (n*cosI - sqrtf(cosT2))*N;
				vec3 rcol( 0, 0, 0);
				raytrace(scene,shadowContrib,ray_t(hitPoint+epsilon*T, T), recursionDepth+1, rcol, rIndex);
				
				if (rcol.x > 1.f) rcol.x = 1.f;
				if (rcol.y > 1.f) rcol.y = 1.f;
				if (rcol.z > 1.f) rcol.z = 1.f;
				
				acc = acc + 0.8f * rcol;
			}
		}
	} else {
		acc = acc + vec3(51/255.f,171/255.f,249/255.f);
	}

}

/*
takes a vector of pixels - the image to write to

creates a camera basis, a scene, traces the scene.
*/
void trace( const Scene &scene, const Camera_t &camera, std::vector<std::vector<Pixel_t>> &pixels, int width, int height )
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


	size_t num_lights = scene.lights.size();
	float shadowContrib = 0.7f / num_lights;

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
			raytrace( scene, shadowContrib, ray, 1, final );
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
				raytrace( scene, shadowContrib, ray, 1, final );
			}
			final = final / 9.f;
#endif

			pixels[x][y] = vec2color(final);
		} // for y

		//printf("x = %d\n", x);
	} //for x
}
