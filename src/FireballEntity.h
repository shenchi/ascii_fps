#pragma once

#include "MeshEntity.h"

class FireballEntity : public MeshEntity
{
public:
	FireballEntity();

	virtual void OnCreate();
	virtual void OnUpdate(float deltaTime);

	inline void SetDirection(float x, float y, float z) { dirX = x; dirY = y; dirZ = z; }

private:
	float dirX, dirY, dirZ;
	float speed;
};
