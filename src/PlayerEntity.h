#pragma once

#include "Entity.h"

class Camera;
class DungeonMap;

class PlayerEntity : public Entity
{
public:
	virtual void OnCreate();
	virtual void OnUpdate(float deltaTime);

	void		SetMap(DungeonMap* map);

private:
	Camera*		camera;
	float		turnSpeed;
	float		stepSpeed;
	float		mouseScaleX;
	float		mouseScaleY;
	DungeonMap*	map;
};
