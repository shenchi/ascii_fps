#include "PlayerEntity.h"
#include "Engine.h"

#include <cmath>

void PlayerEntity::OnCreate()
{
	camera = engine->GetCamera();
	angleHorizontal = 0.0f;

	turnSpeed = 2.0f;
	stepSpeed = 2.0f;

	playerPositionX = 0.0f;
	playerPositionZ = 0.0f;
}

void PlayerEntity::OnUpdate(float deltaTime)
{
	if (engine->IsKeyDown('A')) {
		angleHorizontal += turnSpeed * deltaTime;
	}
	else if (engine->IsKeyDown('D')) {
		angleHorizontal -= turnSpeed * deltaTime;
	}

	if (engine->IsKeyDown('W'))
	{
		playerPositionX += deltaTime * stepSpeed * sin(-angleHorizontal);
		playerPositionZ += deltaTime * stepSpeed * cos(-angleHorizontal);
	}
	else if (engine->IsKeyDown('S'))
	{
		playerPositionX -= deltaTime * stepSpeed * sin(-angleHorizontal);
		playerPositionZ -= deltaTime * stepSpeed * cos(-angleHorizontal);
	}

	float angle = 3.1415f * 0.2f * angleHorizontal;
}
