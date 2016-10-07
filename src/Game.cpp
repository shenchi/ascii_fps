#include "Game.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <cmath>
#include "Engine.h"
#include "EntityManager.h"
#include "DungeonMap.h"
#include "MapEntity.h"
#include "PlayerEntity.h"

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

	engine->CreateEntity("PlayerEntity");

	return engine->Run();
}
