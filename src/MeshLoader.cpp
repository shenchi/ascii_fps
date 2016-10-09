#include "MeshLoader.h"
#include <cstdio>
#pragma warning(disable : 4996)

Mesh MeshLoader::LoadFromFile(const char * filename)
{
	FILE* fp = fopen(filename, "rb");
	
	if (nullptr == fp)
	{
		return Mesh();
	}

	unsigned int num_vertices = 0;
	if (fread(&num_vertices, sizeof(unsigned int), 1, fp) != 1)
	{
		fclose(fp);
		return Mesh();
	}
	
	float* vertices = new float[num_vertices * 9];
	if (fread(vertices, sizeof(float), num_vertices * 9, fp) != num_vertices * 9)
	{
		delete[] vertices;
		fclose(fp);
		return Mesh();
	}

	fclose(fp);

	int* indices = new int[num_vertices];
	for (int i = 0; i < int(num_vertices); ++i)
	{
		indices[i] = i;
	}

	Mesh mesh(num_vertices, 9, num_vertices, vertices, indices);

	delete[] indices;
	delete[] vertices;

	return mesh;
}
