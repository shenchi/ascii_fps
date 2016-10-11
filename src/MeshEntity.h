#pragma once

#include "Entity.h"

struct Mesh;

class MeshEntity : public Entity
{
public:
	MeshEntity();
	virtual ~MeshEntity();

	virtual void LoadMeshFromFile(const char* filename);
	//void SetMesh(Mesh&& mesh);

protected:
	virtual RenderTask*	OnRender();

protected:
	Mesh*		mesh;
	RenderTask*	task;
};
