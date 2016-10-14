#include "BuiltInShaders.h"

#include "Shader.h"

#include <cassert>
#include <glm/glm.hpp>

namespace
{
	using namespace glm;

	/*
	*/
	class DefaultVertexShader : public VertexShader
	{
	public:
		DefaultVertexShader()
			:
			VertexShader(VertexPosition | VertexNormal | VertexColor),
			matMVP(nullptr),
			matWorld(nullptr),
			matView(nullptr),
			matProj(nullptr)
		{}

		virtual void Main(const float* in, float* out) const
		{
			const vec3& pos = reinterpret_cast<const vec3&>(*in);
			const vec3& norm = reinterpret_cast<const vec3&>(*(in + 3));
			const vec3& col = reinterpret_cast<const vec3&>(*(in + 6));

			vec4& pos_out = reinterpret_cast<vec4&>(*out);
			vec4& col_out = reinterpret_cast<vec4&>(*(out + 4));

			vec3 worldNorm = normalize(vec3((*matWorld) * vec4(norm, 0.0f)));

			static vec3 light_dir = normalize(vec3(1.0f, 1.0f, -1.0f));

			pos_out = (*matMVP) * vec4(pos, 1.0f);
			col_out = vec4(col * clamp(dot(worldNorm, light_dir), 0.0f, 1.0f) + vec3(0.5f, 0.5f, 0.5f), 1.0f);
			//col_out = vec4(worldNorm * 0.5f + vec3(0.5f, 0.5f, 0.5f), 1.0f);
			col_out = clamp(col_out, 0.01f, 0.99f);
		}

		virtual void SetConstantBuffer(size_t index, const float* buffer)
		{
			if (index == 0)
			{
				matMVP = reinterpret_cast<const mat4*>(buffer);
				matWorld = reinterpret_cast<const mat4*>(buffer + 16);
				matView = reinterpret_cast<const mat4*>(buffer + 32);
				matProj = reinterpret_cast<const mat4*>(buffer + 48);
			}
		}
	private:
		const mat4* matMVP;
		const mat4* matWorld;
		const mat4* matView;
		const mat4* matProj;
	};
	DefaultVertexShader defaultVertexShader;

	/*
	*/
	class DefaultSkinnedMeshVertexShader : public VertexShader
	{
	public:
		DefaultSkinnedMeshVertexShader()
			:
			VertexShader(VertexPosition | VertexNormal | VertexColor | VertexWeights),
			matMVP(nullptr),
			matWorld(nullptr),
			matView(nullptr),
			matProj(nullptr),
			matBones(nullptr)
		{}

		virtual void Main(const float* in, float* out) const
		{
			const vec3& _pos = reinterpret_cast<const vec3&>(*in);
			const vec4 pos = vec4(_pos, 1.0f);
			const vec3& norm = reinterpret_cast<const vec3&>(*(in + 3));
			const vec3& col = reinterpret_cast<const vec3&>(*(in + 6));

			const uint&	boneId1 = reinterpret_cast<const uint&>(*(in + 9));
			const float& boneWeight1 = *(in + 10);
			const uint&	boneId2 = reinterpret_cast<const uint&>(*(in + 11));
			const float& boneWeight2 = *(in + 12);
			const uint&	boneId3 = reinterpret_cast<const uint&>(*(in + 13));
			const float& boneWeight3 = *(in + 14);
			const uint&	boneId4 = reinterpret_cast<const uint&>(*(in + 15));
			const float& boneWeight4 = *(in + 16);

			vec4& pos_out = reinterpret_cast<vec4&>(*out);
			vec4& col_out = reinterpret_cast<vec4&>(*(out + 4));

			vec3 worldNorm = normalize(vec3((*matWorld) * vec4(norm, 0.0f)));

			static vec3 light_dir = normalize(vec3(1.0f, 1.0f, -1.0f));

			vec4 weightedPos =
				matBones[boneId1] * pos * boneWeight1 +
				matBones[boneId2] * pos * boneWeight2 +
				matBones[boneId3] * pos * boneWeight3 +
				matBones[boneId4] * pos * boneWeight4;

			pos_out = (*matMVP) * weightedPos;

			col_out = vec4(col * clamp(dot(worldNorm, light_dir), 0.0f, 1.0f) + vec3(0.5f, 0.5f, 0.5f), 1.0f);
			col_out = clamp(col_out, 0.01f, 0.99f);
		}

		virtual void SetConstantBuffer(size_t index, const float* buffer)
		{
			if (index == 0)
			{
				matMVP = reinterpret_cast<const mat4*>(buffer);
				matWorld = reinterpret_cast<const mat4*>(buffer + 16);
				matView = reinterpret_cast<const mat4*>(buffer + 32);
				matProj = reinterpret_cast<const mat4*>(buffer + 48);
			}
			else if (index == 2)
			{
				matBones = reinterpret_cast<const mat4*>(buffer);
			}
		}
	private:
		const mat4* matMVP;
		const mat4* matWorld;
		const mat4* matView;
		const mat4* matProj;

		const mat4* matBones;
	};
	DefaultSkinnedMeshVertexShader defaultVertexSkinnedMeshShader;

	class UnlitVertexShader : public VertexShader
	{
	public:
		UnlitVertexShader()
			:
			VertexShader(VertexPosition | VertexNormal | VertexColor),
			matMVP(nullptr),
			matWorld(nullptr),
			matView(nullptr),
			matProj(nullptr)
		{}

		virtual void Main(const float* in, float* out) const
		{
			const vec3& pos = reinterpret_cast<const vec3&>(*in);
			const vec3& norm = reinterpret_cast<const vec3&>(*(in + 3));
			const vec3& col = reinterpret_cast<const vec3&>(*(in + 6));

			vec4& pos_out = reinterpret_cast<vec4&>(*out);
			vec4& col_out = reinterpret_cast<vec4&>(*(out + 4));

			pos_out = (*matMVP) * vec4(pos, 1.0f);
			col_out = vec4(col, 1.0);
		}

		virtual void SetConstantBuffer(size_t index, const float* buffer)
		{
			if (index == 0)
			{
				matMVP = reinterpret_cast<const mat4*>(buffer);
				matWorld = reinterpret_cast<const mat4*>(buffer + 16);
				matView = reinterpret_cast<const mat4*>(buffer + 32);
				matProj = reinterpret_cast<const mat4*>(buffer + 48);
			}
		}
	private:
		const mat4* matMVP;
		const mat4* matWorld;
		const mat4* matView;
		const mat4* matProj;
	};
	UnlitVertexShader unlitVertexShader;

	/*
	*/
	class DefaultPixelShader : public Shader
	{
	public:
		virtual void Main(const float* in, float* out) const
		{
			const vec4& col = reinterpret_cast<const vec4&>(*(in + 4));
			vec4& col_out = reinterpret_cast<vec4&>(*out);
			col_out = col;
		}

		virtual size_t Stride() const { return 8; }

		virtual void SetConstantBuffer(size_t index, const float* buffer)
		{

		}
	};
	DefaultPixelShader defaultPixelShader;


	Shader* vertexShaderArray[] =
	{
		&defaultVertexShader,
		&defaultVertexSkinnedMeshShader,
		&unlitVertexShader,
	};

	size_t registeredVertexShaderCount = sizeof(vertexShaderArray) / sizeof(vertexShaderArray[0]);

	Shader* pixelShaderArray[] =
	{
		&defaultPixelShader,
	};

	size_t registeredPixelShaderCount = sizeof(pixelShaderArray) / sizeof(pixelShaderArray[0]);
}

Shader* BuiltInShaders::DefaultVertexShader()
{
	return &defaultVertexShader;
}

Shader* BuiltInShaders::DefaultPixelShader()
{
	return &defaultPixelShader;
}

Shader* BuiltInShaders::GetVertexShader(size_t idx)
{
	return idx < registeredVertexShaderCount ? vertexShaderArray[idx] : nullptr;
}

Shader * BuiltInShaders::GetVertexShader(VertexShaderIndex idx)
{
	return GetVertexShader(size_t(idx));
}

Shader* BuiltInShaders::GetPixelShader(size_t idx)
{
	return idx < registeredPixelShaderCount ? pixelShaderArray[idx] : nullptr;
}

Shader * BuiltInShaders::GetPixelShader(PixelShaderIndex idx)
{
	return GetPixelShader(size_t(idx));
}