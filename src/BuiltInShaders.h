#pragma once

class Shader;

class BuiltInShaders
{
public:

	enum VertexShaderIndex
	{
		vsDefault = 0,
		vsSkinnedMesh,
		vsUnlit,
		//vsPixelLighting,
	};

	enum PixelShaderIndex
	{
		psDefault = 0,
		//psPixelLighting,
	};

	static Shader*	DefaultVertexShader();
	static Shader*	DefaultPixelShader();

	static Shader*	GetVertexShader(size_t idx);
	static Shader*	GetVertexShader(VertexShaderIndex idx);

	static Shader*	GetPixelShader(size_t idx);
	static Shader*	GetPixelShader(PixelShaderIndex idx);
};
