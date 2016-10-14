#include "ResourceManager.h"

#include "MeshLoader.h"

Mesh* ResourceManager::LoadMesh(const std::string& filename)
{
	if (LoadMeshFile(filename))
	{
		return loadedMeshFiles[filename].mesh;
	}

	return nullptr;
}

Animation* ResourceManager::LoadAnimation(const std::string& filename)
{
	if (LoadMeshFile(filename))
	{
		return loadedMeshFiles[filename].anim;
	}

	return nullptr;
}

bool ResourceManager::LoadMeshFile(const std::string & filename)
{
	if (loadedMeshFiles.find(filename) != loadedMeshFiles.end())
	{
		return true;
	}

	MeshLoader::LoadFromFile(filename.c_str(),
		loadedMeshFiles[filename].mesh,
		loadedMeshFiles[filename].anim);

	if (loadedMeshFiles[filename].mesh->vertices.empty())
	{
		loadedMeshFiles.erase(filename);
		return false;
	}

	if (loadedMeshFiles[filename].anim->bones.empty())
	{
		delete loadedMeshFiles[filename].anim;
		loadedMeshFiles[filename].anim = nullptr;
	}

	return true;
}

ResourceManager::MeshFile::MeshFile()
	:
	mesh(new Mesh()),
	anim(new Animation())
{
}

ResourceManager::MeshFile::MeshFile(MeshFile && other)
	:
	mesh(other.mesh),
	anim(other.anim)
{
	other.mesh = nullptr;
	other.anim = nullptr;
}

ResourceManager::MeshFile::~MeshFile()
{
	if (nullptr != mesh)
		delete mesh;
	if (nullptr != anim)
		delete anim;
}
