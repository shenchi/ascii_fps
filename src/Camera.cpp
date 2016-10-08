#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

using namespace glm;

void Camera::SetLookAt(float x, float y, float z, float upX, float upY, float upZ, float lookAtX, float lookAtY, float lookAtZ)
{
	*reinterpret_cast<mat4*>(viewMatrix) = lookAt(vec3(x, y, z), vec3(lookAtX, lookAtY, lookAtZ), vec3(upX, upY, upZ));
}

void Camera::SetPerspectiveProjection(float fov, float aspect, float zNear, float zFar)
{
	fov = fov / 180.0f * 3.14159265f;
	*reinterpret_cast<mat4*>(projectionMatrix) = perspective(fov, aspect, zNear, zFar);
}

void Camera::SetViewMatrix(const float* matrix)
{
	memcpy(viewMatrix, matrix, sizeof(float) * 16);
}

void Camera::SetProjectionMatrix(const float* matrix)
{
	memcpy(projectionMatrix, matrix, sizeof(float) * 16);
}
