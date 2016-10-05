#pragma once

class Camera
{
public:
	
	void SetPerspectiveProjection(float fov, float aspect, float zNear, float zFar);

private:
	float		projectionMatrix[16];
	float		viewMatrix[16];
};