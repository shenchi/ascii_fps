#include "Game.h"
#include "Engine.h"
#include "EntityManager.h"
// TODO
#include "SkinnedMeshEntity.h"
#include "MapManager.h"

#include "CreaturesData.h"
#include "CreatureManager.h"

// for registration
#include "MapEntity.h"
#include "PlayerEntity.h"
#include "FireballEntity.h"
#include "SlimeEntity.h"

#include <ctime>

Game::Game()
	:engine(new Engine())
{
}

Game::~Game()
{
	delete engine;
}

int Game::Run()
{
	engine->Initialize();

	auto entityMgr = engine->GetEntityManager();

	entityMgr->Register("MapEntity", []() { return new MapEntity(); });
	entityMgr->Register("PlayerEntity", []() { return new PlayerEntity(); });
	entityMgr->Register("FireballEntity", []() { return new FireballEntity(); });
	entityMgr->Register("SlimeEntity", []() { return new SlimeEntity(); });

	CreaturesData::instance()->Init("../assets/creatures.tsv");

	CreatureManager::instance()->SetSeed((unsigned int)std::time(NULL));
	CreatureManager::instance()->SetMaxEnemyCount(10);

	MapManager::instance()->CreateMap();
	
	PlayerEntity* playerEntity = dynamic_cast<PlayerEntity*>(engine->CreateEntity("PlayerEntity"));
	playerEntity->SetPosition(MapManager::instance()->GetStartPositionX(), 1.0f, MapManager::instance()->GetStartPositionY());

	CreatureManager::instance()->SetPlayerEntity(playerEntity);
	CreatureManager::instance()->SetSpawnRadius(20.0f, 40.0f);
	CreatureManager::instance()->SpawnToMaxCount();
	CreatureManager::instance()->SetSpawnRadius(10.0f, 40.0f);

	return engine->Run();
}
