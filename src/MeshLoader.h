#pragma once

#include "Mesh.h"

class MeshLoader
{
public:
	static Mesh LoadFromFile(const char* filename);
};
