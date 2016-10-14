#pragma once

#include "SkinnedMeshEntity.h"
#include "Creature.h"

class SlimeEntity : public SkinnedMeshEntity, public Creature
{
public:
	virtual void OnCreate();

	virtual void OnUpdate(float deltaTime);
};
