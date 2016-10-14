#pragma once

#include <vector>
#include <random>

class Entity;
class PlayerEntity;

class CreatureManager
{
public:
	inline static CreatureManager* instance()
	{
		if (nullptr == _instance)
		{
			_instance = new CreatureManager();
		}
		return _instance;
	}

public:

	inline void SetSeed(unsigned int seed) { rndGen.seed(seed); }
	inline void SetMaxEnemyCount(int count) { maxEnemyCount = count; }
	inline void SetSpawnRadius(float min, float max) { minSpawnRadius = min; maxSpawnRadius = max; }

	inline void SetPlayerEntity(PlayerEntity* entity) { player = entity; }
	inline PlayerEntity* GetPlayerEntity() { return player; }

	void SpawnToMaxCount();
	void SpawnOneEnemy();

private:
	CreatureManager() = default;

private:
	int						maxEnemyCount;
	float					minSpawnRadius;
	float					maxSpawnRadius;
	std::vector<Entity*>	enemies;
	PlayerEntity*			player;


	std::default_random_engine	rndGen;

private:
	static CreatureManager* _instance;
};
