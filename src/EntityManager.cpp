#include "EntityManager.h"

#include "MeshEntity.h"
#include "SkinnedMeshEntity.h"

using namespace std;

EntityManager::EntityManager()
{
	// Register built-in entity types
	Register("MeshEntity", []() { return new MeshEntity(); });
	Register("SkinnedMeshEntity", []() { return new SkinnedMeshEntity(); });
}

EntityManager::~EntityManager()
{
	for (auto i = entities.begin(); i != entities.end(); ++i)
	{
		(*i)->OnDestroy();
		delete *i;
	}
}

Entity * EntityManager::CreateEntity(const std::string & type)
{
	if (entityTable.find(type) == entityTable.end())
	{
		return nullptr;
	}
	Entity* entity = entityTable[type]();
	entities.push_back(entity);
	return entity;
}
