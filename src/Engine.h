#pragma once
#include "Entity.h"
#include "Camera.h"

class ConsoleWindow;
class IColorBufferAdaptor;
class Rasterizer;
class Pipeline;
class EntityManager;
class ResourceManager;

struct Mesh;
struct Animation;

class Engine
{
public:
	inline static Engine*	instance() { return _instance; }
public:
	Engine();
	~Engine();

	int						Initialize();
	int						Run();

	inline void				Quit() { running = false; }

	bool					IsKeyDown(unsigned char keyCode) const;
	int						GetMousePositionX() const;
	int						GetMousePositionY() const;
	int						GetMousePositionDeltaX() const;
	int						GetMousePositionDeltaY() const;

	inline Camera*			GetCamera() { return camera; }

	inline EntityManager*	GetEntityManager() { return entities; }
	Entity*					CreateEntity(const char* type = nullptr);

	Mesh*					LoadMesh(const char* filename);
	Animation*				LoadAnimation(const char* filename);

private:
	bool					running;

	ConsoleWindow*			window;
	IColorBufferAdaptor*	adaptor;
	Rasterizer*				raster;
	Pipeline*				pipeline;
	Camera*					camera;
	EntityManager*			entities;
	ResourceManager*		resources;

private:
	static Engine* _instance;
};
