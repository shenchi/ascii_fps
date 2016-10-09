#pragma once

#include "Entity.h"

class Mesh;

class MeshEntity : public Entity
{
public:
	MeshEntity();
	virtual ~MeshEntity();

	void LoadMeshFromFile(const char* filename);
	void SetMesh(Mesh&& mesh);

protected:
	virtual RenderTask*	OnRender();

private:
	Mesh*		mesh;
	RenderTask*	task;
};
