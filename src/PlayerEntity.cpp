#include "PlayerEntity.h"
#include "Engine.h"
#include "DungeonMap.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <cmath>

void PlayerEntity::OnCreate()
{
	camera = engine->GetCamera();

	mouseScaleX = 0.001f;
	mouseScaleY = 0.001f;
	turnSpeed = 2.0f;
	stepSpeed = 4.0f;
}

void PlayerEntity::OnUpdate(float deltaTime)
{
	constexpr float angle90 = 3.1415926f * 0.5f;
	float anglePitch = GetRotationX();
	float angleYaw = GetRotationY();
	float posX = GetPositionX();
	float posZ = GetPositionZ();

	angleYaw += engine->GetMousePositionDeltaX() * mouseScaleX;
	anglePitch += engine->GetMousePositionDeltaY() * mouseScaleY;

	if (anglePitch > angle90)
		anglePitch = angle90;
	else if (anglePitch < -angle90)
		anglePitch = -angle90;

	if (engine->IsKeyDown('A')) {
		posX -= deltaTime * stepSpeed * cos(angleYaw);
		posZ += deltaTime * stepSpeed * sin(angleYaw);
	}
	else if (engine->IsKeyDown('D')) {
		posX += deltaTime * stepSpeed * cos(angleYaw);
		posZ -= deltaTime * stepSpeed * sin(angleYaw);
	}

	if (engine->IsKeyDown('W'))
	{
		posX += deltaTime * stepSpeed * sin(angleYaw);
		posZ += deltaTime * stepSpeed * cos(angleYaw);
	}
	else if (engine->IsKeyDown('S'))
	{
		posX -= deltaTime * stepSpeed * sin(angleYaw);
		posZ -= deltaTime * stepSpeed * cos(angleYaw);
	}

	map->MoveInMap(posX, posZ, 0.7f);

	if (engine->IsKeyDown(27))
	{
		engine->Quit();
	}
	
	SetPositionX(posX);
	SetPositionZ(posZ);
	SetRotationX(anglePitch);
	SetRotationY(angleYaw);

	UpdateWorldMatrix();

	camera->SetViewMatrix(
		reinterpret_cast<float*>(&(glm::inverse(
			*reinterpret_cast<const glm::mat4*>(GetWorldMatrix())
		)))
	);
}

void PlayerEntity::SetMap(DungeonMap * map)
{
	this->map = map;
}

