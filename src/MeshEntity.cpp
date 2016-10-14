#include "MeshEntity.h"

#include "Engine.h"
#include "Mesh.h"
#include "RenderTask.h"
#include <utility>

MeshEntity::MeshEntity()
	:
	mesh(nullptr),
	task(new RenderTask())
{
	task->worldMatrix = GetWorldMatrix();
	task->mesh = nullptr;
}

MeshEntity::~MeshEntity()
{
	delete task;
}

void MeshEntity::LoadMeshFromFile(const char * filename)
{
	mesh = Engine::instance()->LoadMesh(filename);
	task->mesh = mesh;
}

RenderTask * MeshEntity::OnRender()
{
	if (!Visible() || mesh->vertices == nullptr)
	{
		return nullptr;
	}
	return task;
}
