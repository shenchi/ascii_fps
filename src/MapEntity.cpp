#include "MapEntity.h"
#include "RenderTask.h"

MapEntity::MapEntity()
	:
	Entity(),
	task(new RenderTask())
{
	task->worldMatrix = GetWorldMatrix();
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
	if (Visible() && nullptr != task)
	{
		return task;
	}
	return nullptr;
}
