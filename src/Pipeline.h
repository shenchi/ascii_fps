#pragma once

class Rasterizer;
class Shader;
class PixelEmitter;

class IColorBufferAdaptor
{
public:
	virtual void WriteRenderTarget(int x, int y, const float* color) = 0;
	virtual int GetBufferWidth() const = 0;
	virtual int GetBufferHeight() const = 0;
	virtual void ClearColorBuffer(const float* color) = 0;
};

class Pipeline
{
public:
	Pipeline(Rasterizer* rasterizer, IColorBufferAdaptor* adaptor);
	~Pipeline();

	void Clear(const float* color, float depth);
	void ClearDepthBuffer(float depth);

	void SetViewport(int x, int y, int width, int height);

	void SetVertexShader(Shader* shader);
	void SetPixelShader(Shader* shader);

	void SetConstantBuffer(const float* buffer);

	int Draw(const float* vertices, size_t numVertices, const size_t* indices, size_t numIndices);

private:
	friend class PixelEmitter;

	bool ZTest(int x, int y, float z);

private:
	Rasterizer*				rasterizer;
	IColorBufferAdaptor*	colorBufferAdaptor;
	int						bufferWidth;
	int						bufferHeight;
	float*					depthBuffer;
	Shader*					vertexShader;
	Shader*					pixelShader;
	size_t					vertexDataStride;
	size_t					pixelDataStride;
	float*					pixelDataBlock;
	float					viewportXScale;
	float					viewportXOffset;
	float					viewportYScale;
	float					viewportYOffset;
};

