#pragma once

#include "vec_math.h"

struct Material
{
	bool isTransparent;

	virtual vec3 shade( const vec3 &lightPos, const vec3 &pos, const vec3 &normal ) = 0;
	//virtual vec3 shadeLight( const vec3 &lightPos, const vec3 &pos, const vec3 &normal ) = 0;
};

struct MatNormal: public Material
{
	virtual vec3 shade( const vec3 &lightPos, const vec3 &pos, const vec3 &normal )
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
	
	virtual vec3 shade( const vec3 &lightPos, const vec3 &pos, const vec3 &normal )
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

	MatPhong(const vec3 &diffuseColor) : diffuseColor(diffuseColor)
	{
	}
	virtual vec3 shade( const vec3 &lightPos, const vec3 &pos, const vec3 &normal )
	{
		const vec3 toLight = ( lightPos-pos ).normalize();
		return diffuseColor * std::max( normal.dot( toLight ), 0.f ); // N.L
	}
};