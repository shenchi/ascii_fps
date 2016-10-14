#pragma once

#include "VertexFormat.h"

class Shader
{
public:
	virtual void	Main(const float* in, float* out) const = 0;
	virtual	size_t	Stride() const = 0;

	virtual void	SetConstantBuffer(size_t index, const float* buffer) = 0;
};

class VertexShader : public Shader
{
public:
	VertexShader(unsigned int vertexAttributes) : format(vertexAttributes) {}

	virtual	size_t	Stride() const;

protected:
	VertexFormat	format;
};

class BuiltInShaders
{
public:

	enum VertexShaderIndex
	{
		vsDefault = 0,
		vsSkinnedMesh,
		vsUnlit,
		
	};

	enum PixelShaderIndex
	{
		psDefault = 0,
	};

	static Shader*	DefaultVertexShader();
	static Shader*	DefaultPixelShader();

	static Shader*	GetVertexShader(size_t idx);
	static Shader*	GetVertexShader(VertexShaderIndex idx);

	static Shader*	GetPixelShader(size_t idx);
	static Shader*	GetPixelShader(PixelShaderIndex idx);
};