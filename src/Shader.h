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
