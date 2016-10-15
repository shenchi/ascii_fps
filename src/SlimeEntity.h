#pragma once

#include "SkinnedMeshEntity.h"
#include "Creature.h"

class SlimeEntity : public SkinnedMeshEntity, public Creature
{
public:
	virtual void OnCreate();
	virtual void OnUpdate(float deltaTime);

	virtual void OnDamage(int damage);
	virtual void OnDie();
	virtual void OnHit(HitInfo* hitInfo);

private:
	float jumpCoolDown = 0.0f;
	float attackCoolDown = 0.0f;
	float flyingTimer = 0.0f;
	float flyingDirX, flyingDirY, flyingDirZ;
};
