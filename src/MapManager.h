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
	inline void	SetSeed(unsigned int seed) { this->seed = seed; }

	void	CreateMap();
	void	DestroyMap();

	void	SetVisible(bool visible);

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
	unsigned int		seed = 0;

private:
	static MapManager*	_instance;
};