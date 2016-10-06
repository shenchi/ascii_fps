#include "Engine.h"
#include "ConsoleWindow.h"
#include "ConsoleWindowAdaptor.h"
#include "Rasterizer.h"
#include "Pipeline.h"
#include "EntityManager.h"
#include "Camera.h"
#include <chrono>

namespace
{
	typedef std::chrono::high_resolution_clock timer;
}

Engine::Engine()
	:
	running(false),
	window(nullptr),
	adaptor(nullptr),
	raster(nullptr),
	pipeline(nullptr),
	entities(nullptr),
	camera(nullptr)
{
}

Engine::~Engine()
{
	if (nullptr != camera) delete camera;
	if (nullptr != entities) delete entities;
	if (nullptr != pipeline) delete pipeline;
	if (nullptr != raster) delete raster;
	if (nullptr != adaptor) delete adaptor;
	if (nullptr != window)
	{
		window->Destroy();
		delete window;
	}
}

int Engine::Initialize()
{
	// TODO: read some parameters from config file

	int ret = window->Create(L"Test Console API", 800 / 4, 600 / 8, 4, 8);		// low res
	//int ret = window.Create(L"Test Console API", 1200 / 3, 900 / 6, 3, 6);	// high res

	adaptor = new ConsoleWindowAdaptor(window);
	raster = new Rasterizer(adaptor->GetBufferWidth(), adaptor->GetBufferHeight());
	pipeline = new Pipeline(raster, adaptor);

	entities = new EntityManager();

	camera = new Camera();
	camera->SetPerspectiveProjection(60.f, 0.5f * adaptor->GetBufferWidth() / adaptor->GetBufferHeight(), 0.5f, 100.0f);

	return ret;
}

int Engine::Run()
{
	float interval = 0.016f;
	float elapsed = 0.0f;
	auto lastTime = timer::now();
	char title[256] = {};

	// TODO: put this into camera
	float backgroundColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	running = true;
	while (running)
	{
		window->Update();

		auto now = timer::now();
		float deltaTime = std::chrono::duration<float>(now - lastTime).count();

		if (elapsed + deltaTime < interval)
		{
			continue;
		}

		lastTime = now;
		elapsed = deltaTime + elapsed - interval;

		sprintf_s(title, "delta time: %.4f, mouse position: (%7d, %7d)", deltaTime, window->GetMousePositionX(), window->GetMousePositionY());
		window->SetTitleA(title);


		// Update
		for (auto entity = entities->Begin(); entity != entities->End(); ++entity)
		{
			(*entity)->dirty = false;
			(*entity)->modified = false;
			(*entity)->OnUpdate(deltaTime);
		}

		for (auto entity = entities->Begin(); entity != entities->End(); ++entity)
		{
			(*entity)->UpdateMatrix();
		}

		pipeline->Clear(backgroundColor, 1.0f);

		// Render


		window->Flush();
		window->SwapBuffers();
	}

	return 0;
}

Entity * Engine::CreateEntity(const char* type)
{
	if (nullptr == type)
	{
		return entities->CreateEntity<Entity>();
	}
	return entities->CreateEntity(type);
}

