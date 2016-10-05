#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

using namespace glm;

void Camera::SetPerspectiveProjection(float fov, float aspect, float zNear, float zFar)
{
	fov = fov / 180.0f * 3.14159265f;
	*reinterpret_cast<mat4*>(projectionMatrix) = perspective(fov, aspect, zNear, zFar);
}
