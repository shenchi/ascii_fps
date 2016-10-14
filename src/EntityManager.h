#pragma once
#include "Entity.h"

//#include <vector>
#include <list>
#include <functional>
#include <string>
#include <unordered_map>

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

public:
	// use such interface in case we wanna change underlying implementation
	typedef std::list<Entity*>		ListType;
	typedef ListType::iterator		Iterator;

	inline Iterator Begin() { return entities.begin(); }
	inline Iterator End() { return entities.end(); }

	//
	typedef std::function<Entity*(void)> EntityFactory;
	inline void Register(const std::string& type, EntityFactory factory) { entityTable.insert({type, factory}); }

private:
	friend class Engine;

	inline void CleanUp() { entities.remove_if([](const Entity* e) { return e->remove; }); }

	template<class T>
	inline Entity* CreateEntity() {
		T* entity = new T();
		entities.push_back(entity);
		return entity;
	}

	Entity* CreateEntity(const std::string& type);

private:
	ListType										entities;
	std::unordered_map<std::string, EntityFactory>	entityTable;
};
