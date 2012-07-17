#pragma once

#include "vec_math.h"

struct Material
{
	virtual vec3 shade( const vec3 &pos, const vec3 &normal ) = 0;
};

struct MatNormal: public Material
{
	virtual vec3 shade( const vec3 &pos, const vec3 &normal )
	{
		return 0.5f + 0.5f * normal;
	}
};

struct MatChequered : public Material
{
	vec3 onColor;
	vec3 offColor;
	MatChequered( const vec3 &onColor, const vec3 &offColor )
		: onColor(onColor), offColor(offColor)
	{
	}
	
	virtual vec3 shade( const vec3 &pos, const vec3 &normal )
	{
		float scale = 0.5f;
		const vec3 chequer = scale * pos;
		int sum = int(chequer.x) + int(chequer.y) + int(chequer.z);
		bool flip = pow(-1.0, sum) > 0.0;
		
		if ( flip ) return onColor;
		// else:
		return offColor;
	}
};

struct MatPhong : public Material
{
	vec3 lightDir;

	MatPhong()
	{
		lightDir = vec3( 1.0f, 1.0f, -2.0f ).normalize();
	}
	virtual vec3 shade( const vec3 &pos, const vec3 &normal )
	{
		return vec3(1.f,0.f,0.f) * std::max( normal.dot( lightDir ), 0.f ); // N.L
	}
};