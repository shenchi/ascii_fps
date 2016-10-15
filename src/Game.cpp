#include "Game.h"
#include "Engine.h"
#include "EntityManager.h"
#include "CreaturesData.h"

// for registration
#include "GamePlayLogicEntity.h"
#include "MapEntity.h"
#include "PlayerEntity.h"
#include "FireballEntity.h"
#include "SlimeEntity.h"

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

	entityMgr->Register("GamePlayLogicEntity", []() { return new GamePlayLogicEntity(); });
	entityMgr->Register("MapEntity", []() { return new MapEntity(); });
	entityMgr->Register("PlayerEntity", []() { return new PlayerEntity(); });
	entityMgr->Register("FireballEntity", []() { return new FireballEntity(); });
	entityMgr->Register("SlimeEntity", []() { return new SlimeEntity(); });

	CreaturesData::instance()->Init("../assets/creatures.tsv");

	GamePlayLogicEntity* gameplay = dynamic_cast<GamePlayLogicEntity*>(engine->CreateEntity("GamePlayLogicEntity"));

	return engine->Run();
}
