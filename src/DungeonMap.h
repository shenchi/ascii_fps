#pragma once

class MapGenerator;
struct Mesh;

class DungeonMap
{
public:
	DungeonMap();
	~DungeonMap();

	void Create();

	float GetStartPositionX() const;
	float GetStartPositionY() const;

	void MoveInMap(float& destX, float& destY, float radius) const;

	inline const Mesh* GetMesh() const { return mesh; }

private:
	MapGenerator		*mapGen;
	Mesh				*mesh;

	int					mapWidth;
	int					mapHeight;
	char*				mapData;
};
