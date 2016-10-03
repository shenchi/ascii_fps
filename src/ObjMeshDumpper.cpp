#ifdef _DEBUG
#pragma warning(disable : 4996) // debug code, it's ok
#include "ObjMeshDumpper.h"

#include <cassert>
#include <cstdio>
#include <vector>
#include <string>

using namespace std;

namespace Debugger
{

	struct ObjMeshDumpper::MeshData
	{
		FILE*			fp;
		string			filename;
		vector<ObjVertex>	vertices;
	};

	ObjMeshDumpper::ObjMeshDumpper(const char * filename) : data(new MeshData()), dirty(false)
	{
		data->filename = filename;
		data->fp = fopen(filename, "w");
		assert(nullptr != data->fp);
	}

	ObjMeshDumpper::~ObjMeshDumpper()
	{
		Flush();
		fclose(data->fp);
		delete data;
	}

	void ObjMeshDumpper::AddTriangle(const float * triangle)
	{
		AddTriangle(triangle, triangle + 3, triangle + 6);
	}

	void ObjMeshDumpper::AddTriangle(const float * v1, const float * v2, const float * v3)
	{
		data->vertices.push_back(ObjVertex(v1));
		data->vertices.push_back(ObjVertex(v2));
		data->vertices.push_back(ObjVertex(v3));
		dirty = true;
	}

	void ObjMeshDumpper::AddTriangle(const ObjVertex* triangle)
	{
		AddTriangle(triangle[0], triangle[1], triangle[2]);
	}

	void ObjMeshDumpper::AddTriangle(const ObjVertex& v1, const ObjVertex& v2, const ObjVertex& v3)
	{
		data->vertices.push_back(v1);
		data->vertices.push_back(v2);
		data->vertices.push_back(v3);
		dirty = true;
	}

	void ObjMeshDumpper::Flush()
	{
		if (!dirty)
			return;

		data->fp = freopen(data->filename.c_str(), "w", data->fp);
		assert(nullptr != data->fp);
		assert(data->vertices.size() % 3 == 0);

		fprintf(data->fp, "# vertices\n");

		for (auto v = data->vertices.begin(); v != data->vertices.end(); ++v)
		{
			fprintf(data->fp, "v %10f %10f %10f %10f\n", v->x, v->y, v->z, v->w);
		}

		fprintf(data->fp, "# indices\n");

		for (size_t i = 0; i < data->vertices.size(); i += 3)
		{
			fprintf(data->fp, "f %5d %5d %5d\n", int(i + 1), int(i + 2), int(i + 3));
		}

		dirty = false;
	}
}


#endif

