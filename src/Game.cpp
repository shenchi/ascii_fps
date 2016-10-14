#include "Game.h"
#include "Engine.h"
#include "EntityManager.h"
// TODO
#include "SkinnedMeshEntity.h"
#include "MapManager.h"

// for registration
#include "MapEntity.h"
#include "PlayerEntity.h"
#include "FireballEntity.h"

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

	MapManager::instance()->CreateMap();
	
	PlayerEntity* playerEntity = dynamic_cast<PlayerEntity*>(engine->CreateEntity("PlayerEntity"));
	playerEntity->SetPosition(MapManager::instance()->GetStartPositionX(), 1.0f, MapManager::instance()->GetStartPositionY());

	MeshEntity* meshEntity = dynamic_cast<MeshEntity*>(engine->CreateEntity("MeshEntity"));
	meshEntity->LoadMeshFromFile("../assets/sword.mesh");
	meshEntity->SetParent(playerEntity);
	meshEntity->SetPosition(0.2f, -0.15f, 0.6f);
	meshEntity->SetScale(0.2f, 0.2f, 0.2f);

	SkinnedMeshEntity* slime = dynamic_cast<SkinnedMeshEntity*>(engine->CreateEntity("SkinnedMeshEntity"));
	slime->LoadMeshFromFile("../assets/slime.mesh");
	slime->SetScale(0.5f, 0.5f, 0.5f);
	slime->SetPosition(MapManager::instance()->GetStartPositionX(), 0.0f, MapManager::instance()->GetStartPositionY() + 5.0f);
	slime->SetLoop(true);
	slime->Play();

	return engine->Run();
}
