#pragma once

constexpr size_t MaxLightCount = 16;

struct Light
{
	enum LightType {
		None,
		Directional,
		Point,
	};

	LightType type;
	bool inUse;
	float x, y, z, w;
	float r, g, b, a;
};
