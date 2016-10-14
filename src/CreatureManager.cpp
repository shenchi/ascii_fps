#include "CreatureManager.h"

#include "Engine.h"

#include "MapManager.h"
#include "CreaturesData.h"
#include "Creature.h"

#include "PlayerEntity.h"

#include <string>
#include <cassert>

using namespace std;

CreatureManager* CreatureManager::_instance = nullptr;

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

	creature->SetHP(CreaturesData::instance()->GetIntValue(creatureId, "HP"));
	creature->SetDamage(CreaturesData::instance()->GetIntValue(creatureId, "DAMAGE"));
	creature->SetSpeed(CreaturesData::instance()->GetFloatValue(creatureId, "SPEED"));
	creature->SetRadius(CreaturesData::instance()->GetFloatValue(creatureId, "SIZE"));

	MapManager::instance()->CollideWithMap(spawnX, spawnZ, creature->GetRadius());

	enemy->SetPosition(spawnX, 0.25f, spawnZ);

	enemies.push_back(enemy);
}
