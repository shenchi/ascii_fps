#pragma once

class Mesh
{
public:
	Mesh();
	Mesh(size_t verticesNum, size_t vertexStride, size_t indicesNum);
	Mesh(size_t verticesNum, size_t vertexStride, size_t indicesNum, const float* verticesData, const int* indicesData);
	~Mesh();

	Mesh(const Mesh&) = delete;
	Mesh& operator = (const Mesh&) = delete;

	Mesh(Mesh&&);
	Mesh& operator = (Mesh&&);

	inline const float* GetVerticesData() const { return vertices; }
	inline size_t GetVerticesNum() const { return verticesNum; }

	inline const int* GetIndicesData() const { return indices; }
	inline size_t GetIndicesNum() const { return indicesNum; }

	static Mesh Cube();

private:
	float*		vertices;
	size_t		verticesNum;
	int*		indices;
	size_t		indicesNum;
};
