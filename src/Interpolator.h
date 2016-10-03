#pragma once

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

class Interpolator
{
public:
	inline Interpolator(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3)
		:
		v2(v2), v3(v3),
		v2_3(v3 - v2),
		v3_1(v1 - v3),
		area(glm::cross(v3_1, -v2_3).length())
	{

	}

	inline void Interpolate(const glm::vec3& v, const float* data1, const float* data2, const float* data3, float* data_interp, size_t count)
	{
		float a = glm::cross(v - v2, v2_3).length() / area;
		float b = glm::cross(v - v3, v3_1).length() / area;
		float c = 1 - a - b;

		for (size_t i = 0; i < count; i++)
		{
			data_interp[i] = a * data1[i] + b * data2[i] + c * data3[i];
		}
	}

private:
	const glm::vec3&	v2;
	const glm::vec3&	v3;
	glm::vec3			v2_3;
	glm::vec3			v3_1;
	float				area;
};
