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
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f
	};

	mapMesh.GenerateMesh(gridSize, 2.0f, color_tables);

	const auto verts = mapMesh.GetVertices();
	const auto idxs = mapMesh.GetIndices();

	mesh = new Mesh(verts.size(), 9, idxs.size(), reinterpret_cast<const float*>(&(verts[0])), &(idxs[0]));
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
