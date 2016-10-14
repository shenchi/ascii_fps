#pragma once

#include "Entity.h"

#include "Creature.h"

class Camera;

class PlayerEntity : public Entity, Creature
{
public:
	virtual void OnCreate();
	virtual void OnUpdate(float deltaTime);
	virtual void OnOverlay();

private:
	Camera*		camera;
	float		turnSpeed;
	float		stepSpeed;
	float		mouseScaleX;
	float		mouseScaleY;
	float		fireCoolDown;
};
