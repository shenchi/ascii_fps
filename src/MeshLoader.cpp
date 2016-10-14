#include "MeshLoader.h"
#include "VertexFormat.h"
#include <cstdio>
#pragma warning(disable : 4996)

#define READ_UINT(x) if (fread(&(x), sizeof(unsigned int), 1, fp) != 1) { return false; }

namespace
{

	class File
	{
	public:
		File(const char* filename, const char* mode)
			:
			fp(nullptr)
		{
			fp = fopen(filename, mode);
		}

		~File()
		{
			if (nullptr != fp)
				fclose(fp);
			fp = nullptr;
		}

		operator FILE*()
		{
			return fp;
		}

	private:
		FILE* fp;
	};
}

bool MeshLoader::LoadFromFile(const char* filename, Mesh* mesh, Animation* anim)
{
	File fp(filename, "rb");
	
	if (nullptr == fp)
	{
		return false;
	}

	unsigned int uint_buf;

	// magicn number check
	READ_UINT(uint_buf);
	if (uint_buf != 0x4853454d)
	{
		return false;
	}

	// attribute
	READ_UINT(uint_buf);
	mesh->format = VertexFormat(uint_buf);

	// vertex number, bone number, action number
	unsigned int num_vertices = 0;
	unsigned int num_bones = 0;
	unsigned int num_actions = 0;
	READ_UINT(num_vertices);
	READ_UINT(num_bones);
	READ_UINT(num_actions);

	size_t num_floats = num_vertices * mesh->format.Stride();
	mesh->verticesNum = num_vertices;
	mesh->vertices = ArrayBuffer<float>(num_floats);
	if (fread(mesh->vertices, sizeof(float), num_floats, fp) != num_floats)
	{
		return false;
	}

	mesh->indices = ArrayBuffer<int>();

	if (nullptr == anim || num_bones == 0)
	{
		return true; // mesh loading finished
	}

	anim->bones = ArrayBuffer<int>(num_bones);
	if (fread(anim->bones, sizeof(int), num_bones, fp) != num_bones)
	{
		return false;
	}
	
	if (num_actions == 0)
	{
		return true;
	}

	anim->actions = ArrayBuffer<Action>(num_actions);

	for (size_t a = 0; a < num_actions; ++a)
	{
		Action& action = anim->actions[a];
		READ_UINT(action.startFrame);
		READ_UINT(action.endFrame);
		unsigned int num_keyframes;
		READ_UINT(num_keyframes);

		action.keyFrames = ArrayBuffer<KeyFrame>(num_keyframes);
		for (unsigned int f = 0; f < num_keyframes; ++f)
		{
			KeyFrame& frame = action.keyFrames[f];
			READ_UINT(frame.frame);
			frame.bones = ArrayBuffer<Bone>(num_bones);
			if (fread(frame.bones, sizeof(Bone), num_bones, fp) != num_bones)
			{
				return false;
			}
		}
	}

	return true;
}

//Mesh Mesh::Cube()
//{
//	constexpr float k = 0.5773502691896258f;
//
//	float vertices[] =
//	{
//		-0.5f, -0.5f, -0.5f, -k, -k, -k, 0.0f, 0.0f, 0.0f,
//		-0.5f,  0.5f, -0.5f, -k, k, -k, 0.0f, 1.0f, 0.0f,
//		0.5f, -0.5f, -0.5f, k, -k, -k, 1.0f, 0.0f, 0.0f,
//		0.5f,  0.5f, -0.5f, k, k, -k, 1.0f, 1.0f, 0.0f,
//		-0.5f, -0.5f, 0.5f, -k, -k, k, 0.0f, 0.0f, 1.0f,
//		-0.5f,  0.5f, 0.5f, -k, k, k, 0.0f, 1.0f, 1.0f,
//		0.5f, -0.5f, 0.5f, k, -k, k, 1.0f, 0.0f, 1.0f,
//		0.5f,  0.5f, 0.5f, k, k, k, 1.0f, 1.0f, 1.0f,
//	};
//	size_t num_vertices = sizeof(vertices) / sizeof(float) / 4;
//
//	int indices[] =
//	{
//		0, 1, 2,
//		2, 1, 3,
//
//		2, 3, 6,
//		6, 3, 7,
//
//		6, 7, 4,
//		4, 7, 5,
//
//		4, 5, 0,
//		0, 5, 1,
//
//		1, 5, 3,
//		3, 5, 7,
//
//		4, 0, 6,
//		6, 0, 2
//	};
//	size_t num_indices = sizeof(indices) / sizeof(int);
//	return Mesh(num_vertices, 9, num_indices, vertices, indices);
//}
//