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

	float GetEndPositionX() const;
	float GetEndPositionY() const;

	bool MoveInMap(float& destX, float& destY, float radius) const;
	bool IsInExit(float x, float y) const;
	bool IsWalkable(float x, float y) const;

	inline const Mesh* GetMesh() const { return mesh; }

private:
	MapGenerator		*mapGen;
	Mesh				*mesh;

	int					mapWidth;
	int					mapHeight;
	char*				mapData;
};
