#include "Shader.h"
#include <cassert>
#include <glm/glm.hpp>

namespace
{
	using namespace glm;

	/*
	*/
	class DefaultVertexShader : public Shader
	{
	public:
		virtual void Main(const float* in, float* out) const
		{
			assert(sizeof(vec4) == 4 * 4);
			const vec4& pos = reinterpret_cast<const vec4&>(*in);
			vec4& pos_out = reinterpret_cast<vec4&>(*out);
			vec4& col_out = reinterpret_cast<vec4&>(*(out + 4));

			pos_out = matMVP * pos;
			col_out = pos + vec4(0.5f, 0.5f, 0.5f, 0.0f);
		}

		virtual size_t Stride() const { return 4; }

		virtual void SetConstantBuffer(const float* buffer)
		{
			assert(sizeof(mat4) == 4 * 16);
			matMVP = reinterpret_cast<const mat4&>(*buffer);
		}
	private:
		mat4 matMVP;
	};
	DefaultVertexShader defaultVertexShader;

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

		virtual void SetConstantBuffer(const float* buffer)
		{

		}
	};
	DefaultPixelShader defaultPixelShader;
}


Shader* BuiltInShaders::DefaultVertexShader()
{
	return &defaultVertexShader;
}

Shader* BuiltInShaders::DefaultPixelShader()
{
	return &defaultPixelShader;
}
