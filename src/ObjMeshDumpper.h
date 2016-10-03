#pragma once

#ifdef _DEBUG

namespace Debugger
{
	struct ObjVertex
	{
		float x, y, z, w;

		ObjVertex(const float* vertex) : x(vertex[0]), y(vertex[1]), z(vertex[2]), w(1.0f) {}

		ObjVertex(float x, float y, float z) : x(x), y(y), z(z), w(1.0f) {}

		ObjVertex(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

		inline ObjVertex& applyW() {
			x /= w;
			y /= w;
			z /= w;
			return *this;
		}
	};

	class ObjMeshDumpper
	{
	public:
		ObjMeshDumpper(const char* filename);
		~ObjMeshDumpper();

		void AddTriangle(const float* triangle);
		void AddTriangle(const float* v1, const float* v2, const float* v3);
		void AddTriangle(const ObjVertex* triangle);
		void AddTriangle(const ObjVertex& v1, const ObjVertex& v2, const ObjVertex& v3);

		void Flush();

	private:
		struct MeshData;
		MeshData* data;

		bool dirty;
	};
}


#endif
