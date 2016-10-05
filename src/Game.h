#pragma once

class Engine;

class Game
{
public:
	Game();
	~Game();

	int Run();

private:
	Engine*		engine;
};