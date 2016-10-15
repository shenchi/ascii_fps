#pragma once

#include "Engine.h"

class GamePlayLogicEntity : public Entity
{
public:
	inline static GamePlayLogicEntity* instance() { return _instance; }
public:

	virtual void OnCreate();
	virtual void OnUpdate(float deltaTime);
	virtual void OnDestroy();

	virtual void OnOverlay();

	void InitScene();

	void UpdateMainScreen();
	void UpdateIntroScreen();
	void UpdateWinScreen();
	void UpdateLoseScreen();

	inline bool IsGameWin() const { return gameWin; }
	inline bool IsGameLose() const { return gameLose; }
	inline bool IsGameFinished() const { return gameFinished; }

	inline void SetGameWin() {
		if (!gameFinished) {
			gameFinished = true;
			gameWin = true;
		}
	}

	inline void SetGameLose() {
		if (!gameFinished) {
			gameFinished = true;
			gameLose = true;
		}
	}

private:
	bool gameWin = false;
	bool gameLose = false;
	bool gameFinished = false;

	enum GameState
	{
		MainScreen,
		Intro,
		Scene,
		Cleaning,
		EndingWin,
		EndingLose
	};

	GameState state;

	float timer;

private:
	static GamePlayLogicEntity* _instance;
};