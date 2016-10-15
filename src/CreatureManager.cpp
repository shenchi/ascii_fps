#include "CreatureManager.h"

#include "Engine.h"

#include "MapManager.h"
#include "CreaturesData.h"
#include "Creature.h"

#include "PlayerEntity.h"

#include <string>
#include <cassert>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

CreatureManager* CreatureManager::_instance = nullptr;

void CreatureManager::CreatePlayerEntity()
{
	player = dynamic_cast<PlayerEntity*>(Engine::instance()->CreateEntity("PlayerEntity"));
	player->SetPosition(MapManager::instance()->GetStartPositionX(), 1.0f, MapManager::instance()->GetStartPositionY());
	player->SetMaxHP(CreaturesData::instance()->GetIntValue(0, "HP"));
	player->SetDamage(CreaturesData::instance()->GetIntValue(0, "DAMAGE"));
	player->SetSpeed(CreaturesData::instance()->GetFloatValue(0, "SPEED"));
	player->SetRadius(CreaturesData::instance()->GetFloatValue(0, "SIZE"));
}

void CreatureManager::SpawnToMaxCount()
{
	while (enemies.size() < maxEnemyCount)
	{
		SpawnOneEnemy();
	}
}

void CreatureManager::SpawnOneEnemy()
{
	if (enemies.size() > maxEnemyCount)
		return;

	uniform_real_distribution<float> distRadius(minSpawnRadius, maxSpawnRadius);
	uniform_real_distribution<float> distAngle(0, 2 * 3.1415926535f);

	float spawnX, spawnZ;

	do {
		float r = distRadius(rndGen);
		float a = distAngle(rndGen);

		spawnX = player->GetPositionX() + r * sin(a);
		spawnZ = player->GetPositionZ() + r * cos(a);
	} while (!MapManager::instance()->IsWalkable(spawnX, spawnZ));

	int creatureId = 1; 
	string entityType = CreaturesData::instance()->GetStringValue(creatureId, "ENTITY");

	Entity* enemy = Engine::instance()->CreateEntity(entityType.c_str());
	Creature* creature = dynamic_cast<Creature*>(enemy);

	assert(nullptr != creature);

	creature->SetMaxHP(CreaturesData::instance()->GetIntValue(creatureId, "HP"));
	creature->SetDamage(CreaturesData::instance()->GetIntValue(creatureId, "DAMAGE"));
	creature->SetSpeed(CreaturesData::instance()->GetFloatValue(creatureId, "SPEED"));
	creature->SetRadius(CreaturesData::instance()->GetFloatValue(creatureId, "SIZE"));

	MapManager::instance()->CollideWithMap(spawnX, spawnZ, creature->GetRadius());

	enemy->SetPosition(spawnX, 0.0f, spawnZ);

	enemies.push_back(enemy);
}

void CreatureManager::KillEnemy(Entity* entity, bool respawn)
{
	entity->Destroy();
	auto iter = find(enemies.begin(), enemies.end(), entity);
	if (iter != enemies.end())
	{
		swap(*iter, enemies.back());
		enemies.pop_back();
	}
	if (respawn)
	{
		SpawnToMaxCount();
	}
}

void CreatureManager::KillAllEnemy()
{
	for (auto i = enemies.begin(); i != enemies.end(); ++i)
	{
		(*i)->Destroy();
	}
	enemies.clear();
}

bool CreatureManager::CollideWithPlayer(float x, float y, float z, float radius, HitInfo * hitInfo)
{
	return CollideWithCreature(player, x, y, z, radius, hitInfo);
}

bool CreatureManager::CollideWithEnemies(float x, float y, float z, float radius, HitInfo * hitInfo)
{
	for (auto entity = enemies.begin(); entity != enemies.end(); ++entity)
	{
		if (CollideWithCreature(*entity, x, y, z, radius, hitInfo))
		{
			return true;
		}
	}
	return false;
}

void CreatureManager::ShutDown()
{
	KillAllEnemy();
	player->Destroy();
	player = nullptr;
}

bool CreatureManager::CollideWithCreature(Entity* entity, float x, float y, float z, float radius, HitInfo * hitInfo)
{
	Creature* creature = dynamic_cast<Creature*>(entity);
	if (nullptr == creature)
		return false;
	
	vec3 pos1 = vec3(x, 0, z);
	vec3 pos2 = vec3(entity->GetPositionX(), 0, entity->GetPositionZ());

	vec3 delta = (pos1 - pos2);
	float cRadius = creature->GetRadius();
	if (length(delta) > cRadius + radius)
		return false;

	if (nullptr != hitInfo)
	{
		vec3& hitPos = *reinterpret_cast<vec3*>(&(hitInfo->hitX));
		vec3& hitDir = *reinterpret_cast<vec3*>(&(hitInfo->dirX));

		hitInfo->entity = entity;
		hitDir = normalize(delta);
		hitPos = pos2 + hitDir * cRadius;
	}

	return true;
}

