#include "MapManager.h"

#include "Engine.h"
#include "DungeonMap.h"
#include "MapEntity.h"
#include "MeshEntity.h"

MapManager* MapManager::_instance = nullptr;

void MapManager::CreateMap()
{
	if (nullptr != map)
		return;

	map = new DungeonMap();
	map->Create();
	mapEntity = dynamic_cast<MapEntity*>(Engine::instance()->CreateEntity("MapEntity"));
	mapEntity->SetMesh(map->GetMesh());

	ladderEntity = dynamic_cast<MeshEntity*>(Engine::instance()->CreateEntity("MeshEntity"));
	ladderEntity->LoadMeshFromFile("../assets/ladder.mesh");
	ladderEntity->SetPosition(map->GetEndPositionX(), 0, map->GetEndPositionY());
}

void MapManager::DestroyMap()
{
	if (nullptr == map)
		return;

	delete map;
	mapEntity->Destroy();
	ladderEntity->Destroy();
}

float MapManager::GetStartPositionX() const
{
	if (nullptr != map)
		return map->GetStartPositionX();
	return 0.0f;
}

float MapManager::GetStartPositionY() const
{
	if (nullptr != map)
		return map->GetStartPositionY();
	return 0.0f;
}

bool MapManager::CollideWithMap(float & x, float & y, float radius) const
{
	return map->MoveInMap(x, y, radius);
}

bool MapManager::IsInExit(float x, float y) const
{
	return map->IsInExit(x, y);
}
