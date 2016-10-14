#pragma once

#include <string>
#include <unordered_map>

struct Mesh;
struct Animation;

class ResourceManager
{
public:

	Mesh*		LoadMesh(const std::string& filename);
	Animation*	LoadAnimation(const std::string& filename);

private:

	bool		LoadMeshFile(const std::string& filename);

	struct MeshFile
	{
		Mesh*		mesh;
		Animation*	anim;

		MeshFile();
		MeshFile(const MeshFile&) = delete;
		MeshFile(MeshFile&&);
		~MeshFile();
	};

	std::unordered_map<std::string, MeshFile>	loadedMeshFiles;
};
