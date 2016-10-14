#pragma once

class Rasterizer;
class Shader;
class PixelEmitter;
struct Mesh;
class IColorBufferAdaptor;

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

	void SetConstantBuffer(size_t index, const float* buffer);

	int Draw(const float* vertices, size_t numVertices, const int* indices, size_t numIndices);
	int Draw(const Mesh* mesh);

private:
	friend class PixelEmitter;

	bool ZTest(int x, int y, float z);
	void Clip(const float* inputList, size_t inputCount, float* outputList, size_t& outputCount, size_t stride) const;

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
	float*					clippedDataBlock;
	float*					interpolateDataBlock;
	float					viewportXScale;
	float					viewportXOffset;
	float					viewportYScale;
	float					viewportYOffset;
};

