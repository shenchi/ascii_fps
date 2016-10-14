#include "Engine.h"
#include "ConsoleWindow.h"
#include "ConsoleWindowAdaptor.h"
#include "ConsoleWindowPrinter.h"
#include "Rasterizer.h"
#include "Pipeline.h"
#include "Shader.h"
#include "EntityManager.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "RenderTask.h"
#include "Animation.h"
#include <chrono>
#include <vector>
#include <glm/glm.hpp>

namespace
{
	typedef std::chrono::high_resolution_clock timer;
}

Engine* Engine::_instance = nullptr;

Engine::Engine()
	:
	running(false),
	window(nullptr),
	printer(nullptr),
	adaptor(nullptr),
	raster(nullptr),
	pipeline(nullptr),
	entities(nullptr),
	camera(nullptr),
	resources(nullptr)
{
	assert(_instance == nullptr);
	_instance = this;
}

Engine::~Engine()
{
	if (nullptr != resources) delete resources;
	if (nullptr != camera) delete camera;
	if (nullptr != entities) delete entities;
	if (nullptr != pipeline) delete pipeline;
	if (nullptr != raster) delete raster;
	if (nullptr != adaptor) delete adaptor;
	if (nullptr != printer) delete printer;
	if (nullptr != window)
	{
		window->Destroy();
		delete window;
	}
}

int Engine::Initialize()
{
	// TODO: read some parameters from config file
	window = new ConsoleWindow();
	int ret = window->Create(L"Test Console API", 800 / 4, 600 / 8, 4, 8);		// low res
	//int ret = window.Create(L"Test Console API", 1200 / 3, 900 / 6, 3, 6);	// high res

	printer = new ConsoleWindowPrinter(window);
	adaptor = new ConsoleWindowAdaptor(window);
	raster = new Rasterizer(adaptor->GetBufferWidth(), adaptor->GetBufferHeight());
	pipeline = new Pipeline(raster, adaptor);

	entities = new EntityManager();

	resources = new ResourceManager();

	camera = new Camera();
	camera->SetPerspectiveProjection(45.0f, 0.5f * adaptor->GetBufferWidth() / adaptor->GetBufferHeight(), 0.5f, 80.0f);

	return ret;
}

int Engine::Run()
{
	float interval = 0.016f;
	float elapsed = 0.0f;
	auto lastTime = timer::now();
	char title[256] = {};

	// TODO maybe it should be a list of rendertask
	std::vector<RenderTask*> renderList;

	// TODO: put this into camera
	float backgroundColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	glm::mat4 builtInMatrix[4] = {
		glm::mat4(1.0f),	// MVP
		glm::mat4(1.0f),	// World
		glm::mat4(1.0f),	// View
		glm::mat4(1.0f)		// Projection
	};

	glm::mat4& matMVP = builtInMatrix[0];
	glm::mat4& matWorld = builtInMatrix[1];
	glm::mat4& matView = builtInMatrix[2];
	glm::mat4& matProj = builtInMatrix[3];

	running = true;
	while (running)
	{
		auto now = timer::now();
		float deltaTime = std::chrono::duration<float>(now - lastTime).count();

		if (elapsed + deltaTime < interval)
		{
			continue;
		}

		lastTime = now;
		elapsed = deltaTime + elapsed - interval;

		window->Update();

		sprintf_s(title, "delta time: %.4f, mouse position: (%7d, %7d)", deltaTime, window->GetMousePositionX(), window->GetMousePositionY());
		window->SetTitleA(title);

		renderList.clear();

		// Find all entities that need to be removed, and call OnDestroy()
		for (auto entity = entities->Begin(); entity != entities->End(); ++entity)
		{
			(*entity)->UpdateRemoveFlag();
			if ((*entity)->remove)
				(*entity)->OnDestroy();
		}
		entities->CleanUp();

		// Update
		for (auto entity = entities->Begin(); entity != entities->End(); ++entity)
		{
			(*entity)->OnUpdate(deltaTime);
		}

		// Update all position informations
		for (auto entity = entities->Begin(); entity != entities->End(); ++entity)
		{
			(*entity)->UpdateWorldMatrix();
		}

		// TODO frustrum culling? visibility culling? depth-based sroting?
		for (auto entity = entities->Begin(); entity != entities->End(); ++entity)
		{
			(*entity)->dirty = false;
			(*entity)->modified = false;

			RenderTask* task = (*entity)->OnRender();
			if (nullptr != task)
			{
				renderList.push_back(task);
			}
		}

		pipeline->Clear(backgroundColor, 1.0f);

		// Update Camera
		matView = *reinterpret_cast<glm::mat4*>(camera->viewMatrix);
		matProj = *reinterpret_cast<glm::mat4*>(camera->projectionMatrix);
		glm::mat4 matVP = matProj * matView;

		// Render
		for (auto task = renderList.begin(); task != renderList.end(); ++task)
		{
			matWorld = (*reinterpret_cast<const glm::mat4*>((*task)->worldMatrix));
			matMVP = matVP * matWorld;


			pipeline->SetVertexShader(BuiltInShaders::GetVertexShader((*task)->vertexShader));
			pipeline->SetPixelShader(BuiltInShaders::GetPixelShader((*task)->pixelShader));

			pipeline->SetConstantBuffer(0, reinterpret_cast<float*>(builtInMatrix));

			if ((*task)->pose != nullptr) 
			{
				pipeline->SetConstantBuffer(1, (*task)->pose->matrices);
			}

			pipeline->Draw((*task)->mesh);
		}

		// 
		for (auto entity = entities->Begin(); entity != entities->End(); ++entity)
		{
			(*entity)->OnOverlay();
		}

		window->Flush();
		window->SwapBuffers();
	}

	return 0;
}

bool Engine::IsKeyDown(unsigned char keyCode) const
{
	return window->IsKeyDown(keyCode);
}

int Engine::GetMousePositionX() const
{
	return window->GetMousePositionX();
}

int Engine::GetMousePositionY() const
{
	return window->GetMousePositionY();
}

int Engine::GetMousePositionDeltaX() const
{
	return window->GetMousePositionDeltaX();
}

int Engine::GetMousePositionDeltaY() const
{
	return window->GetMousePositionDeltaY();
}

void Engine::PrintText(int x, int y, const char * string, int attribute)
{
	if (nullptr == printer)
		return;

	if (x < 0) x = adaptor->GetBufferWidth() + x;
	if (y < 0) y = adaptor->GetBufferHeight() + y;

	printer->Print(x, y, string, attribute);
}

Entity* Engine::CreateEntity(const char* type)
{
	Entity* entity = nullptr;
	if (nullptr == type)
	{
		entity = entities->CreateEntity<Entity>();
	}
	entity = entities->CreateEntity(type);

	entity->engine = this;
	entity->OnCreate();
	return entity;
}

Mesh * Engine::LoadMesh(const char * filename)
{
	return resources->LoadMesh(filename);
}

Animation * Engine::LoadAnimation(const char * filename)
{
	return resources->LoadAnimation(filename);
}

