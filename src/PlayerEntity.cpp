#include "PlayerEntity.h"

#include "Engine.h"
#include "MapManager.h"
#include "CreatureManager.h"
#include "FireballEntity.h"
#include "GamePlayLogicEntity.h"

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

	MeshEntity* meshEntity = dynamic_cast<MeshEntity*>(engine->CreateEntity("MeshEntity"));
	meshEntity->LoadMeshFromFile("../assets/sword.mesh");
	meshEntity->SetParent(this);
	meshEntity->SetPosition(0.2f, -0.15f, 0.6f);
	meshEntity->SetScale(0.2f, 0.2f, 0.2f);
}

void PlayerEntity::OnUpdate(float deltaTime)
{
	constexpr float angle90 = 3.1415926f * 0.5f;
	vec3 pos = vec3(GetPositionX(), GetPositionY(), GetPositionZ());

	if (HP < MaxHP)
	{
		recoverTimer += deltaTime;
		if (recoverTimer > 1.0f)
		{
			recoverTimer -= 1.0f;
			HP++;
		}
	}

	if (flyingTimer > 0.0f)
	{
		flyingTimer -= deltaTime;
		vec3 dir = vec3(flyingDirX, 0.0f, flyingDirZ);
		pos += dir * 4.0f * deltaTime;
		MapManager::instance()->CollideWithMap(pos.x, pos.y, Radius);
		SetPositionX(pos.x);
		SetPositionZ(pos.z);
	}
	else
	{
		float anglePitch = GetRotationX();
		float angleYaw = GetRotationY();
		angleYaw += engine->GetMousePositionDeltaX() * mouseScaleX;
		anglePitch += engine->GetMousePositionDeltaY() * mouseScaleY;

		if (anglePitch > angle90)
			anglePitch = angle90;
		else if (anglePitch < -angle90)
			anglePitch = -angle90;

		if (engine->IsKeyDown('A')) {
			pos.x -= deltaTime * Speed * cos(angleYaw);
			pos.z += deltaTime * Speed * sin(angleYaw);
		}
		else if (engine->IsKeyDown('D')) {
			pos.x += deltaTime * Speed * cos(angleYaw);
			pos.z -= deltaTime * Speed * sin(angleYaw);
		}

		if (engine->IsKeyDown('W'))
		{
			pos.x += deltaTime * Speed * sin(angleYaw);
			pos.z += deltaTime * Speed * cos(angleYaw);
		}
		else if (engine->IsKeyDown('S'))
		{
			pos.x -= deltaTime * Speed * sin(angleYaw);
			pos.z -= deltaTime * Speed * cos(angleYaw);
		}

		MapManager::instance()->CollideWithMap(pos.x, pos.z, Radius);

		if (MapManager::instance()->IsInExit(pos.x, pos.z))
		{
			MapManager::instance()->SetVisible(false);
			CreatureManager::instance()->ShutDown();
			GamePlayLogicEntity::instance()->SetGameWin();
		}

		SetPositionX(pos.x);
		SetPositionZ(pos.z);
		SetRotationX(anglePitch);
		SetRotationY(angleYaw);
	}

	UpdateWorldMatrix();

	const mat4& worldMat = *reinterpret_cast<const mat4*>(GetWorldMatrix());

	camera->SetViewMatrix(reinterpret_cast<float*>(&(inverse(worldMat))));

	if (flyingTimer <= 0.0f)
	{
		if (fireCoolDown > 0.0f)
		{
			fireCoolDown -= deltaTime;
		}
		else if (engine->IsKeyDown(VK_LBUTTON))
		{
			fireCoolDown = 0.5f;

			FireballEntity* fireball = dynamic_cast<FireballEntity*>(engine->CreateEntity("FireballEntity"));
			fireball->SetOwner(this);
			vec4 worldPos = vec4(pos, 1.0f);
			vec4 frontPos = worldMat * vec4(0.0f, 0.0f, 1.0f, 1.0f);
			vec3 dir = normalize(vec3(frontPos - worldPos));
			fireball->SetPosition(frontPos.x, frontPos.y, frontPos.z);
			fireball->SetDirection(dir.x, dir.y, dir.z);
		}
	}
}


#pragma warning(disable: 4996) // we are not using sprintf_s
void PlayerEntity::OnOverlay()
{
	float color[] = { 1.0f, 0.0f, 0.0f };
	char buf[16];
	sprintf(buf, "HP%d", HP);
	Engine::instance()->PrintText(2, -8, buf, false, color);
}

void PlayerEntity::OnDamage(int damage)
{
}

void PlayerEntity::OnDie()
{
	MapManager::instance()->SetVisible(false);
	CreatureManager::instance()->ShutDown();
	GamePlayLogicEntity::instance()->SetGameLose();
}

void PlayerEntity::OnHit(HitInfo * hitInfo)
{
	flyingTimer = 0.5f;
	flyingDirX = -hitInfo->dirX;
	flyingDirY = -hitInfo->dirY;
	flyingDirZ = -hitInfo->dirZ;
}

