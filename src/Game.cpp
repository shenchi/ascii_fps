#include "Game.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <cmath>
#include "Engine.h"
#include "DungeonMap.h"

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

	DungeonMap map;

	map.Create();


	// TODO: Move these to player entity
	//	if (window.IsKeyDown('A')) {
	//		angleHorizontal += turnSpeed * deltaTime;
	//	}
	//	else if (window.IsKeyDown('D')) {
	//		angleHorizontal -= turnSpeed * deltaTime;
	//	}

	//	if (window.IsKeyDown('W'))
	//	{
	//		playerPositionX += deltaTime * stepSpeed * sin(-angleHorizontal);
	//		playerPositionZ += deltaTime * stepSpeed * cos(-angleHorizontal);
	//	}
	//	else if (window.IsKeyDown('S'))
	//	{
	//		playerPositionX -= deltaTime * stepSpeed * sin(-angleHorizontal);
	//		playerPositionZ -= deltaTime * stepSpeed * cos(-angleHorizontal);
	//	}

	//	float angle = 3.1415f * 0.2f * angleHorizontal;

	//	


	// TODO: Move these to a renderer abstract object
	//	pipeline.SetVertexShader(BuiltInShaders::GetVertexShader(1));

	//	glm::mat4& matTranslate = glm::translate(glm::vec3(-playerPositionX, -1.0f, -playerPositionZ));
	//	glm::mat4& matRotate = glm::rotate(angleHorizontal, glm::vec3(0.0f, 1.0f, 0.0f));
	//	glm::mat4 matMVP = matProj * matRotate * matTranslate;
	//	pipeline.SetConstantBuffer(reinterpret_cast<float*>(&matMVP));

	//	pipeline.Draw(map.GetMesh());

	//}

	return engine->Run();
}
