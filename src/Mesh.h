#pragma once

#include "VertexFormat.h"
#include "ArrayBuffer.h"

struct Mesh
{
	Mesh() = default;
	~Mesh() = default;
	Mesh(const Mesh&) = delete;
	Mesh(Mesh&&) = default;

	Mesh& operator = (const Mesh&) = delete;
	Mesh& operator = (Mesh&&) = default;

	inline void Clear()
	{
		verticesNum = 0;
		vertices.release();
		indices.release();
	}

	VertexFormat		format;
	size_t				verticesNum;
	ArrayBuffer<float>	vertices;
	ArrayBuffer<int>	indices;
};
