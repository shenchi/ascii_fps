#include "PlayerEntity.h"
#include "Engine.h"

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
	float playerPositionX = GetPositionX();
	float playerPositionZ = GetPositionZ();

	angleYaw += engine->GetMousePositionDeltaX() * mouseScaleX;
	anglePitch += engine->GetMousePositionDeltaY() * mouseScaleY;

	if (anglePitch > angle90)
		anglePitch = angle90;
	else if (anglePitch < -angle90)
		anglePitch = -angle90;

	if (engine->IsKeyDown('A')) {
		playerPositionX -= deltaTime * stepSpeed * cos(angleYaw);
		playerPositionZ += deltaTime * stepSpeed * sin(angleYaw);
	}
	else if (engine->IsKeyDown('D')) {
		playerPositionX += deltaTime * stepSpeed * cos(angleYaw);
		playerPositionZ -= deltaTime * stepSpeed * sin(angleYaw);
	}

	if (engine->IsKeyDown('W'))
	{
		playerPositionX += deltaTime * stepSpeed * sin(angleYaw);
		playerPositionZ += deltaTime * stepSpeed * cos(angleYaw);
	}
	else if (engine->IsKeyDown('S'))
	{
		playerPositionX -= deltaTime * stepSpeed * sin(angleYaw);
		playerPositionZ -= deltaTime * stepSpeed * cos(angleYaw);
	}

	if (engine->IsKeyDown(27))
	{
		engine->Quit();
	}
	
	SetPositionX(playerPositionX);
	SetPositionZ(playerPositionZ);
	SetRotationX(anglePitch);
	SetRotationY(angleYaw);

	UpdateWorldMatrix();

	camera->SetViewMatrix(
		reinterpret_cast<float*>(&(glm::inverse(
			*reinterpret_cast<const glm::mat4*>(GetWorldMatrix())
		)))
	);
}

