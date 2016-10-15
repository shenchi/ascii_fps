#include "FireballEntity.h"

#include "RenderTask.h"
#include "BuiltInShaders.h"

#include "GamePlayLogicEntity.h"
#include "MapManager.h"
#include "CreatureManager.h"
#include "Creature.h"

#include <glm/glm.hpp>
#include <cassert>

using namespace glm;

FireballEntity::FireballEntity()
{
	task->vertexShader = BuiltInShaders::VertexShaderIndex::vsUnlit;
	speed = 20.0f;
	radius = 0.1f;
}

void FireballEntity::OnCreate()
{
	LoadMeshFromFile("../assets/fireball.mesh");
}

void FireballEntity::OnUpdate(float deltaTime)
{
	if (GamePlayLogicEntity::instance()->IsGameFinished())
	{
		Destroy();
		return;
	}

	vec3& dir = *reinterpret_cast<vec3*>(&dirX);
	vec3 pos = vec3(GetPositionX(), GetPositionY(), GetPositionZ());

	pos += dir * deltaTime * speed;
	
	HitInfo hitInfo;
	if (CreatureManager::instance()->CollideWithEnemies(pos.x, pos.y, pos.z, radius, &hitInfo))
	{
		Creature* attactker = dynamic_cast<Creature*>(owner);
		Creature* attackee = dynamic_cast<Creature*>(hitInfo.entity);
		assert(nullptr != attackee);
		attactker->Attack(attackee);
		hitInfo.dirX = -dirX;
		hitInfo.dirY = -dirY;
		hitInfo.dirZ = -dirZ;
		attackee->OnHit(&hitInfo);
		Destroy();
	}

	if (MapManager::instance()->CollideWithMap(pos.x, pos.z, radius) || pos.y <= 0.0f || pos.y >= 2.0f)
	{
		Destroy();
	}

	SetPosition(pos.x, pos.y, pos.z);
}

