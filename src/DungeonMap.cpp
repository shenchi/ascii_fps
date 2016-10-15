#include "DungeonMap.h"

#include "MapGenerator.h"
#include "MapMesh.h"
#include "Mesh.h"

namespace
{
	constexpr char tileTable[NumTileType] = { ' ', '.', '#' };
	constexpr char tileDensityTable[] = { '.', ' ', '#' };
	constexpr size_t nTileDensities = sizeof(tileDensityTable) / sizeof(tileDensityTable[0]);

	const int genCount = 50;
	const int genRadius = 10;
	const int genMinLength = 3;
	const int genMaxLength = 10;
	const float gridSize = 2.0f;
}

DungeonMap::DungeonMap()
	:
	mapGen(new MapGenerator()),
	mesh(nullptr),
	mapWidth(0),
	mapHeight(0),
	mapData(nullptr)
{
}

DungeonMap::~DungeonMap()
{
	delete mapGen;

	if (nullptr != mesh)
	{
		delete mesh;
	}

	if (nullptr != mapData)
	{
		delete[] mapData;
	}
}

void DungeonMap::SetSeed(unsigned int seed)
{
	mapGen->SetSeed(seed);
}

void DungeonMap::Create()
{
	if (nullptr != mapData)
	{
		return;
	}

	mapGen->Start(genCount, genRadius, genMinLength, genMaxLength);

	//mapGen.SetSeed((int)std::time(NULL));

	while (!mapGen->IsFinished())
	{
		mapGen->Update();
	}
	mapGen->GenEntryAndExit();

	mapWidth = mapGen->Right() - mapGen->Left() + 1;
	mapHeight = mapGen->Bottom() - mapGen->Top() + 1;
	size_t w = mapWidth;
	size_t h = mapHeight;
	mapData = new char[mapWidth * mapHeight];
	mapGen->Gen2DArrayMap(mapData, w, h, tileTable);

	MapMesh mapMesh;
	mapMesh.CreateFromGridMap(mapData, mapWidth, mapHeight, tileDensityTable, nTileDensities, 1);

	// TODO
	float color_tables[] = {
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f
	};

	mapMesh.GenerateMesh(gridSize, 2.0f, color_tables);

	const auto verts = mapMesh.GetVertices();
	const auto idxs = mapMesh.GetIndices();

	mesh = new Mesh();
	mesh->format = VertexFormat(VertexPosition | VertexNormal | VertexColor);
	mesh->verticesNum = verts.size();

	mesh->vertices = ArrayBuffer<float>(verts.size() * mesh->format.Stride());
	memcpy(mesh->vertices, &verts[0], sizeof(float) * mesh->vertices.size());

	mesh->indices = ArrayBuffer<int>(idxs.size());
	memcpy(mesh->indices, &idxs[0], sizeof(int) * idxs.size());
}

float DungeonMap::GetStartPositionX() const
{
	if (nullptr == mapData)
	{
		return 0.0f;
	}
	return gridSize * (mapGen->EntryX() - mapGen->Left() + 0.5f);
}

float DungeonMap::GetStartPositionY() const
{
	if (nullptr == mapData)
	{
		return 0.0f;
	}
	return gridSize * (mapGen->EntryY() - mapGen->Top() + 0.5f);
}

float DungeonMap::GetEndPositionX() const
{
	if (nullptr == mapData)
	{
		return 0.0f;
	}
	return gridSize * (mapGen->ExitX() - mapGen->Left() + 0.5f);
}

float DungeonMap::GetEndPositionY() const
{
	if (nullptr == mapData)
	{
		return 0.0f;
	}
	return gridSize * (mapGen->ExitY() - mapGen->Top() + 0.5f);
}

bool DungeonMap::MoveInMap(float& destX, float& destY, float radius) const
{
	if (nullptr == mapData)
	{
		return false;
	}

	bool collided = false;

#define TEST(OFFSET_X, OFFSET_Y, COORD_VAR, GRID_COORD, COORD_OFFSET)\
	{\
		int dx = int((destX OFFSET_X) / gridSize);\
		int dy = int((destY OFFSET_Y) / gridSize);\
		char destType = mapData[dy * mapWidth + dx];\
		if (destType != '.')\
		{\
			COORD_VAR = (GRID_COORD) * gridSize COORD_OFFSET;\
			collided = true;\
		}\
	}

	TEST(+radius, , destX, dx, -radius);
	TEST(-radius, , destX, dx + 1, +radius);
	TEST(, +radius, destY, dy, -radius);
	TEST(, -radius, destY, dy + 1, +radius);

#undef TEST

	return collided;
}

bool DungeonMap::IsInExit(float x, float y) const
{
	int dx = int(x / gridSize);
	int dy = int(y / gridSize);
	if (dx == (mapGen->ExitX() - mapGen->Left()) &&
		dy == (mapGen->ExitY() - mapGen->Top()))
	{
		return true;
	}
	return false;
}

bool DungeonMap::IsWalkable(float x, float y) const
{
	int dx = int(x / gridSize);
	int dy = int(y / gridSize);
	if (dx < 0 || dx >= mapWidth || dy < 0 || dy >= mapHeight)
		return false;
	return (mapData[dy * mapWidth + dx] == '.');
}
