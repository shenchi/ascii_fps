#pragma once

class DungeonMap;
class MeshEntity;
class MapEntity;

class MapManager
{
public:
	inline static MapManager* instance() {
		if (nullptr == _instance)
		{
			_instance = new MapManager();
		}
		return _instance;
	}

public:

	void	CreateMap();
	void	DestroyMap();

	float	GetStartPositionX() const;
	float	GetStartPositionY() const;
	bool	CollideWithMap(float& x, float& y, float radius) const;
	bool	IsInExit(float x, float y) const;
	bool	IsWalkable(float x, float y) const;

private:
	MapManager() = default;

private:
	DungeonMap*			map;
	MapEntity*			mapEntity;
	MeshEntity*			ladderEntity;

private:
	static MapManager*	_instance;
};