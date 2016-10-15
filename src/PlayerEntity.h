#pragma once

#include "Entity.h"

#include "Creature.h"

class Camera;

class PlayerEntity : public Entity, public Creature
{
public:
	virtual void OnCreate();
	virtual void OnUpdate(float deltaTime);
	virtual void OnOverlay();

	virtual void OnDamage(int damage);
	virtual void OnDie();
	virtual void OnHit(HitInfo* hitInfo);
private:
	Camera*		camera;
	float		turnSpeed;
	float		mouseScaleX;
	float		mouseScaleY;
	float		fireCoolDown;
	float		recoverTimer;
	float		flyingTimer;
	float		flyingDirX, flyingDirY, flyingDirZ;
};
