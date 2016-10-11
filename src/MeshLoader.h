#pragma once

#include "Mesh.h"
#include "Animation.h"

class MeshLoader
{
public:
	static bool LoadFromFile(const char* filename, Mesh* mesh, Animation* anim = nullptr);
};
