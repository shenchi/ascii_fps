#pragma once

#include "Entity.h"
struct RenderTask;
struct Mesh;

class MapEntity : public Entity
{
public:
	MapEntity();
	virtual ~MapEntity();

	virtual void OnUpdate(float deltaTime);

	void SetMesh(const Mesh* mesh);

protected:
	virtual RenderTask* OnRender();

private:
	RenderTask*	task;
};
