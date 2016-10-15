#pragma once

#include "Light.h"

#include "ArrayBuffer.h"

class LightManager
{
public:

	inline Light* GetFreeLight() {
		for (size_t i = 0; i < MaxLightCount; ++i)
		{
			if (!lights[i].inUse)
			{
				lights[i].inUse = true;
				return &lights[i];
			}
		}
		return nullptr;
	}

	inline size_t Count() const { return lights.size(); }
	inline Light* operator[] (const size_t idx) { return &lights[idx]; }

	inline operator Light*() {
		if (lights.empty())
			return nullptr;
		return lights;
	}

private:
	ArrayBuffer<Light>	lights = ArrayBuffer<Light>(MaxLightCount);
};
