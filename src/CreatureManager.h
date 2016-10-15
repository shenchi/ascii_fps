#pragma once

#include <vector>
#include <random>

class Entity;
class PlayerEntity;

struct HitInfo
{
	Entity*	entity;
	float hitX, hitY, hitZ;
	float dirX, dirY, dirZ;
};

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

	void		CreatePlayerEntity();
	inline PlayerEntity* GetPlayerEntity() { return player; }

	void		SpawnToMaxCount();
	void		SpawnOneEnemy();
	void		KillEnemy(Entity* entity, bool respawn = true);
	void		KillAllEnemy();

	bool		CollideWithPlayer(float x, float y, float z, float radius, HitInfo* hitInfo = nullptr);
	bool		CollideWithEnemies(float x, float y, float z, float radius, HitInfo* hitInfo = nullptr);

	void		ShutDown();

private:
	CreatureManager() = default;

	static bool	CollideWithCreature(Entity* entity, float x, float y, float z, float radius, HitInfo* hitInfo);

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
