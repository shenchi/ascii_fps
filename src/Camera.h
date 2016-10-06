#pragma once

class Camera
{
public:
	
	void SetLookAt(float x, float y, float z, float upX, float upY, float upZ, float lookAtX, float lookAtY, float lookAtZ);
	void SetPerspectiveProjection(float fov, float aspect, float zNear, float zFar);


private:
	friend class Engine;
	float		projectionMatrix[16];
	float		viewMatrix[16];
};