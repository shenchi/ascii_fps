#include <Windows.h>
#include "Game.h"

//#define TEST_UNITS

#ifdef TEST_UNITS

#include "Mesh.h"
#include "Animation.h"
#include "MeshLoader.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	// test mesh loader
	Mesh mesh;
	Animation anim;
	if (!MeshLoader::LoadFromFile("assets/slime.mesh", &mesh, &anim))
	{
		MessageBox(nullptr, L"Loading Failed", L"Failed", MB_OK);
		return -1;
	}

	return 0;
}

#else

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	Game game;
	return game.Run();
}

#endif