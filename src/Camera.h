#pragma once

class Camera
{
public:
	
	void SetLookAt(float x, float y, float z, float upX, float upY, float upZ, float lookAtX, float lookAtY, float lookAtZ);
	void SetPerspectiveProjection(float fov, float aspect, float zNear, float zFar);

	void SetViewMatrix(const float* matrix);
	void SetProjectionMatrix(const float* matrix);

private:
	friend class Engine;
	float		viewMatrix[16];
	float		projectionMatrix[16];
};