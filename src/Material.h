#pragma once

#include "vec_math.h"

struct Material
{
	bool isTransparent;
	bool isReflective;

	Material() {
		isTransparent = false;
		isReflective = false;
	}

	virtual vec3 shade( const vec3 &lightPos, const vec3 &pos, const vec3 &normal, const ray_t &ray ) = 0;
	//virtual vec3 shadeLight( const vec3 &lightPos, const vec3 &pos, const vec3 &normal ) = 0;
};

struct MatNormal: public Material
{
	virtual vec3 shade( const vec3 &lightPos, const vec3 &pos, const vec3 &normal, const ray_t &ray )
	{
		return 0.5f + 0.5f * normal;
	}
};

struct MatChequered : public Material
{
	vec3 onColor;
	vec3 offColor;
	float frequency;
	MatChequered( const vec3 &onColor, const vec3 &offColor, float frequency=0.5f )
		: onColor(onColor), offColor(offColor), frequency(frequency)
	{
	}
	
	virtual vec3 shade( const vec3 &lightPos, const vec3 &pos, const vec3 &normal, const ray_t &ray )
	{
		const vec3 chequer = frequency * pos;
		int sum = int( floor(chequer.x) + floor(chequer.y) + floor(chequer.z) );
		
		if ( (sum&1) == 0 ) return onColor;
		// else:
		return offColor;
	}
};

struct MatPhong : public Material
{
	const vec3 diffuseColor;

	MatPhong(const vec3 &diffuseColor) 
		: diffuseColor(diffuseColor)
	{
	}
	virtual vec3 shade( const vec3 &lightPos, const vec3 &pos, const vec3 &normal, const ray_t &ray )
	{
		const vec3 toLight = ( lightPos-pos ).normalize();
		const float nDotL = normal.dot( toLight );
		if ( nDotL > 0 ) { 
			vec3 acc = nDotL * diffuseColor;
			const vec3 &V = ray.dir;
			const vec3 &R = toLight - 2.0f * toLight.dot(normal) * normal;
			float VRVR = V.dot(R);
			if ( VRVR > 0 ) {
				float spc = powf(VRVR, 20.f); // * specular color * shadowAtt
				acc = acc + vec3(spc,spc,spc); // * light color
			}
			return acc;
		}
		return vec3(0.f,0.f,0.f);
		
	}
};