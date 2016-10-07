#pragma once
#include "Entity.h"

#include <vector>
#include <functional>
#include <string>
#include <unordered_map>

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	template<class T>
	inline Entity* CreateEntity() {
		T* entity = new T();
		entities.push_back(entity);
		return entity;
	}

	Entity* CreateEntity(const std::string& type);

	// use such interface in case we wanna change underlying implementation
	typedef std::vector<Entity*>::iterator& Iterator;

	inline Iterator Begin() { return entities.begin(); }
	inline Iterator End() { return entities.end(); }

	//
	typedef std::function<Entity*(void)> EntityFactory;
	inline void Register(const std::string& type, EntityFactory& factory) { entityTable.insert({type, factory}); }

private:
	std::vector<Entity*>							entities;
	std::unordered_map<std::string, EntityFactory>	entityTable;
};
