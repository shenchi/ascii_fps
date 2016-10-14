#pragma once

#include "Light.h"

#include <vector>
#include <algorithm>

class LightManager
{
public:

	inline Light* CreateLight() {
		lights.push_back(Light());
		return &lights[lights.size() - 1];
	}

	inline void	RemoveLights() {
		lights.erase(std::remove_if(lights.begin(), lights.end(), [](const Light& l) { return l.remove == 1; }), lights.end());
	}

	inline size_t Count() const { return lights.size(); }
	inline Light* operator[] (const size_t idx) { return &lights[idx]; }

	inline operator Light*() {
		if (lights.empty())
			return nullptr;
		return &lights[0];
	}

private:
	std::vector<Light>	lights;
};
