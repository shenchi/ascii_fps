#pragma once
#include "Entity.h"
#include "Camera.h"

class ConsoleWindow;
class ConsoleWindowPrinter;
class IColorBufferAdaptor;
class Rasterizer;
class Pipeline;
class EntityManager;
class ResourceManager;
class LightManager;

struct Mesh;
struct Animation;
struct Light;

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

	void					PrintText(int x, int y, const char* string, bool hcentered = false, const float* color = nullptr);

	inline Camera*			GetCamera() { return camera; }

	inline EntityManager*	GetEntityManager() { return entities; }
	Entity*					CreateEntity(const char* type = nullptr);

	Mesh*					LoadMesh(const char* filename);
	Animation*				LoadAnimation(const char* filename);

	Light*					CreateLight();

private:
	bool					running;

	ConsoleWindow*			window;
	ConsoleWindowPrinter*	printer;
	IColorBufferAdaptor*	adaptor;
	Rasterizer*				raster;
	Pipeline*				pipeline;
	Camera*					camera;
	EntityManager*			entities;
	ResourceManager*		resources;
	LightManager*			lights;

private:
	static Engine* _instance;
};
