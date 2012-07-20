#pragma once

#include "vec_math.h"

class Material
{
public:
	bool isTransparent;
	bool castsShadow;

	float kReflect;
	float kRefract;
	float kDiffuse;
	float kSpecular;
	float kRefrectionIndex;

	Material() {
		isTransparent = false;
		castsShadow = true;

		kReflect = 0;
		kRefract = 0;
		kDiffuse = 0.2f;
		kSpecular = 0.8f;
		kRefrectionIndex = 1.5f;
	}

	virtual vec3 shade( const vec3 &lightPos, const vec3 &pos, const vec3 &normal, const ray_t &ray, const float shadowAtt ) = 0;
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

	virtual vec3 getDiffuse(const vec3 &pos) { return color; };

	virtual vec3 shade( const vec3 &lightPos, const vec3 &pos, const vec3 &normal, const ray_t &ray, const float shadowAtt )
	{
		return color;
	}
};

class MatNormal: public Material
{
public:
	virtual vec3 shade( const vec3 &lightPos, const vec3 &pos, const vec3 &normal, const ray_t &ray, const float shadowAtt )
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

	virtual vec3 getDiffuse(const vec3 &pos)
	{
		const vec3 chequer = frequency * pos;
		int sum = int( floor(chequer.x) + floor(chequer.y) + floor(chequer.z) );
		
		if ( (sum&1) == 0 ) return onColor;
		// else:
		return offColor;
	}
	
	virtual vec3 shade( const vec3 &lightPos, const vec3 &pos, const vec3 &normal, const ray_t &ray, const float shadowAtt )
	{
		return getDiffuse(pos);
	}
};

class MatPhong : public Material
{
public:
	vec3 diffuseColor;

	MatPhong() 
		: diffuseColor( vec3(1.f,1.f,1.f) )
	{
	}

	MatPhong(const vec3 &diffuseColor) 
		: diffuseColor(diffuseColor)
	{
	}
	MatPhong(const vec3 &diffuseColor, const float kDiffuse) 
		: diffuseColor(diffuseColor)
	{
	}

	virtual vec3 getDiffuse(const vec3 &pos) { return diffuseColor; };

	virtual vec3 shade( const vec3 &lightPos, const vec3 &pos, const vec3 &normal, const ray_t &ray, const float shadowAtt )
	{
		const vec3 toLight = ( lightPos-pos ).normalize();
		const float nDotL = normal.dot( toLight );
		if ( nDotL > 0.0f ) { 
			const vec3 hitToLight = lightPos-pos;
			//float oneOverDist = 1.0f / hitToLight.length();
			float diff = nDotL * kDiffuse * shadowAtt;
			vec3 acc = diff * getDiffuse(pos); //*lightColor
			
			if ( kSpecular > 0.f ) {
				const vec3 &V = ray.dir;
				const vec3 R = toLight - 2.0f * toLight.dot(normal) * normal;
				float VRVR = V.dot(R);
				if ( VRVR > 0.0f ) {
					float spc = kSpecular * powf(VRVR, 20) * shadowAtt; // * specular color
					acc += spc * vec3(1.f); // * light color
				}
			}
			return acc;
		}

		return vec3(0.f);
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

	virtual vec3 shade( const vec3 &lightPos, const vec3 &pos, const vec3 &normal, const ray_t &ray, const float shadowAtt )
	{
		return MatPhong::shade(lightPos, pos, normal, ray, shadowAtt);
	}
};