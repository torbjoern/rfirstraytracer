#pragma once

struct Light_t
{
	vec3 pos;
	// float kConstantAtt = 1.f;
	//float kLinear; // intensity = 1.0 / distance
	//float kQuadratic; // i = 1.0 / d^2
	Material* mat;

	Light_t( const vec3 &pos ) : pos(pos)
	{
		mat = nullptr;
	}
};