#include "MeshEntity.h"
#include "Mesh.h"
#include "RenderTask.h"
#include "MeshLoader.h"
#include <utility>

MeshEntity::MeshEntity()
	:
	mesh(new Mesh()),
	task(new RenderTask())
{
	task->worldMatrix = GetWorldMatrix();
	task->mesh = mesh;
}

MeshEntity::~MeshEntity()
{
	delete task;
	delete mesh;
}

void MeshEntity::LoadMeshFromFile(const char * filename)
{
	if (!MeshLoader::LoadFromFile(filename, mesh, nullptr))
	{
		mesh->Clear();
	}
}

//void MeshEntity::SetMesh(Mesh&& mesh)
//{
//	*(this->mesh) = std::forward<Mesh>(mesh);
//}

RenderTask * MeshEntity::OnRender()
{
	if (!Visible() || mesh->vertices == nullptr)
	{
		return nullptr;
	}
	return task;
}
