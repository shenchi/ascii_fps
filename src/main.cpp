#include <Windows.h>
#include "Game.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	Game game;
	return game.Run();
}
