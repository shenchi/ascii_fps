#include "GamePlayLogicEntity.h"

#include "MapManager.h"
#include "CreatureManager.h"

#include <Windows.h>
#include <ctime>

GamePlayLogicEntity* GamePlayLogicEntity::_instance = nullptr;

void GamePlayLogicEntity::OnCreate()
{
	_instance = this;
	state = MainScreen;
	timer = 0.0f;
}

void GamePlayLogicEntity::OnUpdate(float deltaTime)
{
	if (engine->IsKeyDown(VK_ESCAPE))
	{
		engine->Quit();
	}

	switch (state)
	{
	case MainScreen:
		if (engine->IsKeyDown(VK_RETURN) || engine->IsKeyDown(VK_SPACE))
		{
			state = Intro;
			timer = 5.0f;
		}
		break;
	case Intro:
		if (timer > 0.0f)
		{
			timer -= deltaTime;
		}
		else
		{
			state = Scene;
			InitScene();
		}
		break;
	case Scene:
		if (gameFinished)
			state = Cleaning;
		break;
	case Cleaning:
		MapManager::instance()->DestroyMap();
		if (gameWin)
			state = EndingWin;
		if (gameLose)
			state = EndingLose;
		break;
	case EndingWin:
		break;
	case EndingLose:
		if (engine->IsKeyDown(VK_RETURN) || engine->IsKeyDown(VK_SPACE))
		{
			MapManager::instance()->SetSeed(static_cast<unsigned int>(std::time(NULL)));
			gameWin = false;
			gameLose = false;
			gameFinished = false;
			timer = 5.0f;
			state = Intro;
		}
		break;
	default:
		break;
	}
}

void GamePlayLogicEntity::OnDestroy()
{
	_instance = nullptr;
}

void GamePlayLogicEntity::OnOverlay()
{
	switch (state)
	{
	case MainScreen:
		UpdateMainScreen();
		break;
	case GamePlayLogicEntity::Intro:
		UpdateIntroScreen();
		break;
	case GamePlayLogicEntity::Scene:
		break;
	case GamePlayLogicEntity::Cleaning:
		break;
	case GamePlayLogicEntity::EndingWin:
		UpdateWinScreen();
		break;
	case GamePlayLogicEntity::EndingLose:
		UpdateLoseScreen();
		break;
	default:
		break;
	}
}

void GamePlayLogicEntity::InitScene()
{
	MapManager::instance()->CreateMap();
	CreatureManager::instance()->CreatePlayerEntity();
	CreatureManager::instance()->SetSeed((unsigned int)std::time(NULL));
	CreatureManager::instance()->SetMaxEnemyCount(10);
	CreatureManager::instance()->SetSpawnRadius(20.0f, 60.0f);
	CreatureManager::instance()->SpawnToMaxCount();
	CreatureManager::instance()->SetSpawnRadius(10.0f, 60.0f);
}



void GamePlayLogicEntity::UpdateMainScreen()
{
	Engine::instance()->PrintText(0, 10, "Dungeon Escapist", true);

	Engine::instance()->PrintText(0, 50, "WSAD + mouse", true);
	Engine::instance()->PrintText(0, 60, "Press enter to continue", true);
}

void GamePlayLogicEntity::UpdateIntroScreen()
{
	float color[] = { 0.0f, 0.0f, 1.0f };

	if (timer < 3.0f) {
		float t = timer / 3.0f;
		if (t < 0.0f) t = 0.0f;
		color[2] = t;
	}

	Engine::instance()->PrintText(0, 20, "Where am I?", true, color);
	Engine::instance()->PrintText(0, 30, "...", true, color);
	Engine::instance()->PrintText(0, 40, "I must find a way out", true, color);
}

void GamePlayLogicEntity::UpdateWinScreen()
{
	float color[] = { 0.0f, 1.0f, 0.0f };
	Engine::instance()->PrintText(0, 20, "Congratulations", true, color);

	Engine::instance()->PrintText(0, 40, "you successfully  ", true, color);
	Engine::instance()->PrintText(0, 50, "   find a way ouy.", true, color);
}

void GamePlayLogicEntity::UpdateLoseScreen()
{
	float color[] = { 1.0f, 0.0f, 0.0f };
	Engine::instance()->PrintText(0, 20, "Game Over", true, color);

	Engine::instance()->PrintText(0, 40, "your strength    ", true, color);
	Engine::instance()->PrintText(0, 50, "   is exhausted...", true, color);
	
}
