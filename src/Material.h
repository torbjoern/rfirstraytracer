#pragma once

#include "vec_math.h"

class Material
{
public:
	bool isTransparent;
	bool isReflective;
	bool castsShadow;

	Material() {
		isTransparent = false;
		isReflective = false;
		castsShadow = true;
	}

	virtual vec3 shade( const vec3 &lightPos, const vec3 &pos, const vec3 &normal, const ray_t &ray ) = 0;
	//virtual vec3 shadeLight( const vec3 &lightPos, const vec3 &pos, const vec3 &normal ) = 0;

	virtual vec3 getDiffuse(const vec3 &pos) { return vec3(0.f,0.f,0.f); };
};

class MatConstant : public Material
{
private:
	const vec3 color;
public:
	MatConstant( const vec3 color ) 
		: color(color) 
	{}
	virtual vec3 shade( const vec3 &lightPos, const vec3 &pos, const vec3 &normal, const ray_t &ray )
	{
		return color;
	}
};

class MatNormal: public Material
{
public:
	virtual vec3 shade( const vec3 &lightPos, const vec3 &pos, const vec3 &normal, const ray_t &ray )
	{
		return 0.5f + 0.5f * normal;
	}
};

class MatChequered : public Material
{
public:
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

class MatPhong : public Material
{
public:
	vec3 diffuseColor;
	const float diffuseK;

	MatPhong() 
		: diffuseColor( vec3(1.f,1.f,1.f) )
		, diffuseK( 1.0f )
	{
	}

	MatPhong(const vec3 &diffuseColor) 
		: diffuseColor(diffuseColor)
		, diffuseK( 1.0f )
	{
	}
	MatPhong(const vec3 &diffuseColor, const float diffuseK) 
		: diffuseColor(diffuseColor)
		, diffuseK( diffuseK )
	{
	}

	virtual vec3 getDiffuse(const vec3 &pos) { return diffuseColor; };

	virtual vec3 shade( const vec3 &lightPos, const vec3 &pos, const vec3 &normal, const ray_t &ray )
	{
		const vec3 toLight = ( lightPos-pos ).normalize();
		const float nDotL = normal.dot( toLight );
		if ( nDotL > 0.0f ) { 
			const vec3 hitToLight = ( lightPos-pos );
			//float oneOverDSq = 1.0f / hitToLight.length();
			vec3 acc = nDotL * this->getDiffuse(pos) * diffuseK;// * oneOverDSq;
			const vec3 &V = ray.dir;
			const vec3 &R = toLight - 2.0f * toLight.dot(normal) * normal;
			float VRVR = V.dot(R);
			if ( VRVR > 0.0f ) {
				float spc = powf(VRVR, 20); // * specular color * shadowAtt
				spc = std::max(0.f, std::min(1.f, spc) );
				acc = acc + spc * vec3(0.8f,0.8f,0.8f); // * light color
			}
			if (acc.x > 1.f) acc.x = 1.f;
			if (acc.y > 1.f) acc.y = 1.f;
			if (acc.z > 1.f) acc.z = 1.f;
			return acc;
		}
		return vec3(0.f,0.f,0.f);
		
	}
};

class MatPhongChequered : public MatPhong
{
public:
	vec3 onColor;
	vec3 offColor;
	float frequency;
	MatPhongChequered( const vec3 &onColor, const vec3 &offColor, float frequency=0.5f )
		: onColor(onColor), offColor(offColor), frequency(frequency)
	{
	}
	
	virtual vec3 getDiffuse(const vec3 &pos)
	{
		const vec3 chequer = frequency * pos;
		int sum = int( floor(chequer.x) + floor(chequer.y) + floor(chequer.z) );
		
		if ( (sum&1) == 0 ) return onColor;
		// else:
		return offColor;
	}

	virtual vec3 shade( const vec3 &lightPos, const vec3 &pos, const vec3 &normal, const ray_t &ray )
	{
		return MatPhong::shade(lightPos, pos, normal, ray);
	}
};