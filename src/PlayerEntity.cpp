#include "PlayerEntity.h"

#include "Engine.h"
#include "MapManager.h"
#include "FireballEntity.h"

#include <Windows.h>
#include <cstdio>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <cmath>

using namespace glm;

void PlayerEntity::OnCreate()
{
	camera = engine->GetCamera();

	mouseScaleX = 0.001f;
	mouseScaleY = 0.001f;
	turnSpeed = 2.0f;
	stepSpeed = 4.0f;

	MeshEntity* meshEntity = dynamic_cast<MeshEntity*>(engine->CreateEntity("MeshEntity"));
	meshEntity->LoadMeshFromFile("../assets/sword.mesh");
	meshEntity->SetParent(this);
	meshEntity->SetPosition(0.2f, -0.15f, 0.6f);
	meshEntity->SetScale(0.2f, 0.2f, 0.2f);
}

void PlayerEntity::OnUpdate(float deltaTime)
{
	constexpr float angle90 = 3.1415926f * 0.5f;
	float anglePitch = GetRotationX();
	float angleYaw = GetRotationY();
	vec3 pos = vec3(GetPositionX(), GetPositionY(), GetPositionZ());

	angleYaw += engine->GetMousePositionDeltaX() * mouseScaleX;
	anglePitch += engine->GetMousePositionDeltaY() * mouseScaleY;

	if (anglePitch > angle90)
		anglePitch = angle90;
	else if (anglePitch < -angle90)
		anglePitch = -angle90;

	if (engine->IsKeyDown('A')) {
		pos.x -= deltaTime * stepSpeed * cos(angleYaw);
		pos.z += deltaTime * stepSpeed * sin(angleYaw);
	}
	else if (engine->IsKeyDown('D')) {
		pos.x += deltaTime * stepSpeed * cos(angleYaw);
		pos.z -= deltaTime * stepSpeed * sin(angleYaw);
	}

	if (engine->IsKeyDown('W'))
	{
		pos.x += deltaTime * stepSpeed * sin(angleYaw);
		pos.z += deltaTime * stepSpeed * cos(angleYaw);
	}
	else if (engine->IsKeyDown('S'))
	{
		pos.x -= deltaTime * stepSpeed * sin(angleYaw);
		pos.z -= deltaTime * stepSpeed * cos(angleYaw);
	}

	MapManager::instance()->CollideWithMap(pos.x, pos.z, 0.7f);
	
	SetPositionX(pos.x);
	SetPositionZ(pos.z);
	SetRotationX(anglePitch);
	SetRotationY(angleYaw);

	UpdateWorldMatrix();

	const mat4& worldMat = *reinterpret_cast<const mat4*>(GetWorldMatrix());

	camera->SetViewMatrix(reinterpret_cast<float*>(&(inverse(worldMat))));

	// == 
	if (fireCoolDown > 0.0f)
	{
		fireCoolDown -= deltaTime;
	}

	if (engine->IsKeyDown(VK_LBUTTON) && fireCoolDown <= 0.0f)
	{
		fireCoolDown = 0.5f;

		FireballEntity* fireball = dynamic_cast<FireballEntity*>(engine->CreateEntity("FireballEntity"));
		vec4 worldPos = vec4(pos, 1.0f);
		vec4 frontPos = worldMat * vec4(0.0f, 0.0f, 1.0f, 1.0f);
		vec3 dir = normalize(vec3(frontPos - worldPos));
		fireball->SetPosition(frontPos.x, frontPos.y, frontPos.z);
		fireball->SetDirection(dir.x, dir.y, dir.z);
	}

	if (engine->IsKeyDown(27))
	{
		engine->Quit();
	}
}

void PlayerEntity::OnOverlay()
{
	char buf[16];
	sprintf(buf, "HP%d", HP);
	Engine::instance()->PrintText(2, -8, buf, 12);
}

