#include "Mesh.h"
#include <cstring>

Mesh::Mesh()
	:
	vertices(nullptr),
	verticesNum(0),
	indices(nullptr),
	indicesNum(0)
{
}

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

Mesh::Mesh(Mesh&& mesh)
	:
	vertices(mesh.vertices),
	verticesNum(mesh.verticesNum),
	indices(mesh.indices),
	indicesNum(mesh.indicesNum)
{
	mesh.vertices = nullptr;
	mesh.indices = nullptr;
}

Mesh& Mesh::operator=(Mesh&& mesh)
{
	if (this != &mesh)
	{
		if (nullptr != vertices)
			delete[] vertices;

		if (nullptr != indices)
			delete[] indices;

		vertices = mesh.vertices;
		verticesNum = mesh.verticesNum;
		indices = mesh.indices;
		indicesNum = mesh.indicesNum;

		mesh.vertices = nullptr;
		mesh.indices = nullptr;
	}

	return *this;
}

Mesh Mesh::Cube()
{
	constexpr float k = 0.5773502691896258f;

	float vertices[] =
	{
		-0.5f, -0.5f, -0.5f, -k, -k, -k, 0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -k, k, -k, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, -0.5f, k, -k, -k, 1.0f, 0.0f, 0.0f,
		0.5f,  0.5f, -0.5f, k, k, -k, 1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, -k, -k, k, 0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f, 0.5f, -k, k, k, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, k, -k, k, 1.0f, 0.0f, 1.0f,
		0.5f,  0.5f, 0.5f, k, k, k, 1.0f, 1.0f, 1.0f,
	};
	size_t num_vertices = sizeof(vertices) / sizeof(float) / 4;

	int indices[] =
	{
		0, 1, 2,
		2, 1, 3,

		2, 3, 6,
		6, 3, 7,

		6, 7, 4,
		4, 7, 5,

		4, 5, 0,
		0, 5, 1,

		1, 5, 3,
		3, 5, 7,

		4, 0, 6,
		6, 0, 2
	};
	size_t num_indices = sizeof(indices) / sizeof(int);
	return Mesh(num_vertices, 9, num_indices, vertices, indices);
}

