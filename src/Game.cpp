#include "Game.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <cmath>
#include "Engine.h"
#include "EntityManager.h"
#include "DungeonMap.h"
#include "MapEntity.h"
#include "PlayerEntity.h"
#include "Mesh.h"
#include "MeshEntity.h"
#include "SkinnedMeshEntity.h"

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

	DungeonMap map;
	map.Create();

	MapEntity* mapEntity = dynamic_cast<MapEntity*>(engine->CreateEntity("MapEntity"));
	mapEntity->SetMesh(map.GetMesh());
	//mapEntity->SetVisible(false);

	PlayerEntity* playerEntity = dynamic_cast<PlayerEntity*>(engine->CreateEntity("PlayerEntity"));
	playerEntity->SetPosition(map.GetStartPositionX(), 1.0f, map.GetStartPositionY());
	playerEntity->SetMap(&map);

	MeshEntity* meshEntity = dynamic_cast<MeshEntity*>(engine->CreateEntity("MeshEntity"));
	meshEntity->LoadMeshFromFile("../assets/sword.mesh");
	meshEntity->SetParent(playerEntity);
	meshEntity->SetPosition(0.2f, -0.15f, 0.6f);
	meshEntity->SetScale(0.2f, 0.2f, 0.2f);

	SkinnedMeshEntity* slime = dynamic_cast<SkinnedMeshEntity*>(engine->CreateEntity("SkinnedMeshEntity"));
	slime->LoadMeshFromFile("../assets/slime.mesh");
	slime->SetScale(0.5f, 0.5f, 0.5f);
	slime->SetPosition(map.GetStartPositionX(), 0.0f, map.GetStartPositionY() + 5.0f);
	slime->SetLoop(true);
	slime->Play();

	return engine->Run();
}
