#pragma once

class Shader
{
public:
	virtual void	Main(const float* in, float* out) const = 0;
	virtual size_t	Stride() const = 0;

	virtual void	SetConstantBuffer(const float* buffer) = 0;
};

class BuiltInShaders
{
public:
	static Shader*	DefaultVertexShader();

	static Shader*	DefaultPixelShader();

	static Shader*	GetVertexShader(size_t idx);

	static Shader*	GetPixelShader(size_t idx);
};