#pragma once
#include "Entity.h"
#include "Camera.h"

class ConsoleWindow;
class IColorBufferAdaptor;
class Rasterizer;
class Pipeline;
class EntityManager;

class Engine
{
public:
	Engine();
	~Engine();

	int Initialize();
	int Run();

	inline void				Quit() { running = false; }

	inline Camera*			GetCamera() { return camera; }
	Entity*					CreateEntity(const char* type = nullptr);

private:
	bool					running;

	ConsoleWindow*			window;
	IColorBufferAdaptor*	adaptor;
	Rasterizer*				raster;
	Pipeline*				pipeline;
	Camera*					camera;
	EntityManager*			entities;
};