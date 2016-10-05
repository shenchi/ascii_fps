#pragma once

class MapGenerator;
class Mesh;

class DungeonMap
{
public:
	DungeonMap();
	~DungeonMap();

	void Create();

	float GetStartPositionX() const;
	float GetStartPositionY() const;

	inline const Mesh* GetMesh() const { return mesh; }

private:
	MapGenerator		*mapGen;
	Mesh				*mesh;

	int					mapWidth;
	int					mapHeight;
	char*				mapData;
};
