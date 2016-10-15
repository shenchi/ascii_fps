#include "SlimeEntity.h"

#include "CreatureManager.h"
#include "MapManager.h"
#include "PlayerEntity.h"

#include <glm/glm.hpp>

using namespace glm;

void SlimeEntity::OnCreate()
{
	LoadMeshFromFile("../assets/slime.mesh");
	SetFrameRate(40);
}

void SlimeEntity::OnUpdate(float deltaTime)
{
	if (attackCoolDown > 0.0f)
	{
		attackCoolDown -= deltaTime;
	}

	if (flyingTimer > 0.0f)
	{
		flyingTimer -= deltaTime;

		vec2 pos = vec2(GetPositionX(), GetPositionZ());
		vec2 dir = vec2(flyingDirX, flyingDirZ);
		pos += dir * 5.0f * deltaTime;
		MapManager::instance()->CollideWithMap(pos.x, pos.y, Radius);
		SetPositionX(pos.x);
		SetPositionZ(pos.y);
		return;
	}
	if (!IsPlaying())
	{
		jumpCoolDown -= deltaTime;

		if (jumpCoolDown < 0.0f)
		{
			Rewind();
			Play();
			jumpCoolDown = 1.0f;
		}
	}
	else
	{
		PlayerEntity* player = CreatureManager::instance()->GetPlayerEntity();
		vec2 playerPos = vec2(player->GetPositionX(), player->GetPositionZ());
		vec2 pos = vec2(GetPositionX(), GetPositionZ());

		vec2 delta = playerPos - pos;
		vec2 dir = normalize(delta);

		pos += dir * Speed * deltaTime;

		HitInfo hitInfo;
		if (attackCoolDown <= 0.0f && CreatureManager::instance()->CollideWithPlayer(
			GetPositionX(), GetPositionY(), GetPositionZ(), Radius, &hitInfo))
		{
			attackCoolDown = 2.0f;
			Attack(player);
			hitInfo.dirX = -dir.x;
			hitInfo.dirY = 0;
			hitInfo.dirZ = -dir.y;
			player->OnHit(&hitInfo);
			hitInfo.dirX = dir.x;
			hitInfo.dirY = 0;
			hitInfo.dirZ = dir.y;
			OnHit(&hitInfo);
		}

		MapManager::instance()->CollideWithMap(pos.x, pos.y, Radius);

		SetPositionX(pos.x);
		SetPositionZ(pos.y);
	}

	SkinnedMeshEntity::OnUpdate(deltaTime);
}

void SlimeEntity::OnDamage(int damage)
{
}

void SlimeEntity::OnDie()
{
	CreatureManager::instance()->KillEnemy(this);
}

void SlimeEntity::OnHit(HitInfo * hitInfo)
{
	flyingTimer = 0.5f;
	flyingDirX = -hitInfo->dirX;
	flyingDirY = -hitInfo->dirY;
	flyingDirZ = -hitInfo->dirZ;
}
