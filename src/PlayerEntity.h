#pragma once

#include "Entity.h"

class Camera;

class PlayerEntity : public Entity
{
public:
	virtual void OnCreate();
	virtual void OnUpdate(float deltaTime);

private:
	Camera*		camera;
	float		turnSpeed;
	float		stepSpeed;
	float		mouseScaleX;
	float		mouseScaleY;
	float		fireCoolDown;
};
