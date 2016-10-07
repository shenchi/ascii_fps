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

	//========================================================================
	// interface functions for initialization code
	//
	int						Initialize();
	int						Run();


	//========================================================================
	// interface functions for entity code
	//
	inline void				Quit() { running = false; }

	inline Camera*			GetCamera() { return camera; }
	Entity*					CreateEntity(const char* type = nullptr);

	//========================================================================
	// instance getters
	//
	inline EntityManager*	GetEntityManager() { return entities; }

private:
	bool					running;

	ConsoleWindow*			window;
	IColorBufferAdaptor*	adaptor;
	Rasterizer*				raster;
	Pipeline*				pipeline;
	Camera*					camera;
	EntityManager*			entities;
};
