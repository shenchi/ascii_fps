#include "SlimeEntity.h"

#include "CreatureManager.h"
#include "MapManager.h"
#include "PlayerEntity.h"

#include <glm/glm.hpp>

using namespace glm;

void SlimeEntity::OnCreate()
{
	LoadMeshFromFile("../assets/slime.mesh");
	SetScale(0.25f, 0.25f, 0.25f);
	
	// TEST
	SetLoop(true);
	Play();
}

void SlimeEntity::OnUpdate(float deltaTime)
{
	PlayerEntity* player = CreatureManager::instance()->GetPlayerEntity();
	vec2 playerPos = vec2(player->GetPositionX(), player->GetPositionZ());
	vec2 pos = vec2(GetPositionX(), GetPositionZ());

	vec2 delta = playerPos - pos;
	vec2 dir = normalize(delta);

	pos += dir * Speed * deltaTime;

	MapManager::instance()->CollideWithMap(pos.x, pos.y, Radius);

	SetPositionX(pos.x);
	SetPositionZ(pos.y);

	SkinnedMeshEntity::OnUpdate(deltaTime);
}
