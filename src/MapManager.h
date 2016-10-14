#pragma once

class DungeonMap;
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

private:
	MapManager() = default;

private:
	DungeonMap*			map;
	MapEntity*			mapEntity;

private:
	static MapManager*	_instance;
};