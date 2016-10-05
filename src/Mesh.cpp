#include "Mesh.h"
#include <cstring>

Mesh::Mesh(size_t verticesNum, size_t vertexStride, size_t indicesNum)
	:
	vertices(new float[verticesNum * vertexStride]),
	verticesNum(verticesNum),
	indices(new int[indicesNum]),
	indicesNum(indicesNum)
{
}

Mesh::Mesh(size_t verticesNum, size_t vertexStride, size_t indicesNum, const float* verticesData, const int* indicesData)
	: Mesh(verticesNum, vertexStride, indicesNum)
{
	memcpy(vertices, verticesData, sizeof(float) * verticesNum * vertexStride);
	memcpy(indices, indicesData, sizeof(int) * indicesNum);
}

Mesh::~Mesh()
{
	if (nullptr != vertices)
		delete[] vertices;

	if (nullptr != indices)
		delete[] indices;
}
