#include "MapEntity.h"
#include "RenderTask.h"

MapEntity::MapEntity()
	:
	Entity(),
	task(new RenderTask())
{
	task->worldMatrix = worldMatrix;
}

MapEntity::~MapEntity()
{
	delete task;
}

void MapEntity::OnUpdate(float deltaTime)
{
}

void MapEntity::SetMesh(const Mesh * mesh)
{
	task->mesh = mesh;
}

RenderTask* MapEntity::OnRender()
{
	if (nullptr != task)
	{
		return task;
	}
	return nullptr;
}
