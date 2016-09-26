#include "Pipeline.h"
#include "Shader.h"
#include "Rasterizer.h"
#include "PixelEmitter.h"

Pipeline::Pipeline(Rasterizer* rasterizer, IColorBufferAdaptor* adaptor)
	:
	rasterizer(rasterizer),
	colorBufferAdaptor(adaptor),
	bufferWidth(adaptor->GetBufferWidth()),
	bufferHeight(adaptor->GetBufferHeight()),
	depthBuffer(new float[bufferWidth * bufferHeight]),
	vertexShader(nullptr),
	pixelShader(nullptr),
	vertexDataStride(0),
	pixelDataStride(0),
	pixelDataBlock(nullptr)
{
	SetVertexShader(nullptr);
	SetPixelShader(nullptr);
	SetViewport(0, 0, bufferWidth, bufferHeight);
}


Pipeline::~Pipeline()
{
	if (nullptr != pixelDataBlock)
	{
		delete[] pixelDataBlock;
	}

	delete[] depthBuffer;
}

void Pipeline::Clear(const float * color, float depth)
{
	colorBufferAdaptor->ClearColorBuffer(color);
	ClearDepthBuffer(depth);
}

void Pipeline::ClearDepthBuffer(float depth)
{
	size_t len = bufferWidth * bufferHeight;
	for (size_t i = 0; i < len; ++i)
	{
		depthBuffer[i] = depth;
	}
}

void Pipeline::SetViewport(int x, int y, int width, int height)
{
	viewportXScale = (width - 1) * 0.5f;
	viewportXOffset = viewportXScale + x;
	viewportYScale = (height - 1) * 0.5f;
	viewportYOffset = viewportYScale + y;
}

void Pipeline::SetVertexShader(Shader * shader)
{
	if (nullptr == shader)
	{
		shader = BuiltInShaders::DefaultVertexShader();
	}
	vertexShader = shader;
	vertexDataStride = vertexShader->Stride();
}

void Pipeline::SetPixelShader(Shader * shader)
{
	if (nullptr == shader)
	{
		shader = BuiltInShaders::DefaultPixelShader();
	}
	pixelShader = shader;
	pixelDataStride = pixelShader->Stride();

	if (nullptr != pixelDataBlock)
	{
		delete[] pixelDataBlock;
	}
	pixelDataBlock = new float[pixelDataStride * 4]; // TODO multi-threaded
}

void Pipeline::SetConstantBuffer(const float * buffer)
{
	vertexShader->SetConstantBuffer(buffer);
	pixelShader->SetConstantBuffer(buffer);
}

static bool IsCW(float* v1, float* v2, float* v3)
{
	float dx2_1 = v2[0] - v1[0];
	float dy2_1 = v2[1] - v1[1];
	float dx3_1 = v3[0] - v1[0];
	float dy3_1 = v3[1] - v1[1];
	return (dx2_1 * dy3_1 - dy2_1 * dx3_1 > 0);
}

int Pipeline::Draw(const float* vertices, size_t numVertices, const size_t* indices, size_t numIndices)
{
	if (numIndices % 3 != 0)
	{
		return -1;
	}

	float* v1_out = pixelDataBlock;
	float* v2_out = v1_out + pixelDataStride;
	float* v3_out = v2_out + pixelDataStride;

	float screenPositions[6] = {};

	for (size_t i = 0; i < numIndices; i += 3)
	{
		const float* v1_in = vertices + vertexDataStride * indices[i];
		const float* v2_in = vertices + vertexDataStride * indices[i + 1];
		const float* v3_in = vertices + vertexDataStride * indices[i + 2];

		vertexShader->Main(v1_in, v1_out);
		vertexShader->Main(v2_in, v2_out);
		vertexShader->Main(v3_in, v3_out);

		// cull back face (CCW - front face)
		if (IsCW(v1_out, v2_out, v3_out))
		{
			continue;
		}

		screenPositions[0] = (viewportXScale * (v1_out[0] / v1_out[3]) + viewportXOffset);
		screenPositions[1] = (-viewportYScale * (v1_out[1] / v1_out[3]) + viewportYOffset);
		screenPositions[2] = (viewportXScale * (v2_out[0] / v2_out[3]) + viewportXOffset);
		screenPositions[3] = (-viewportYScale * (v2_out[1] / v2_out[3]) + viewportYOffset);
		screenPositions[4] = (viewportXScale * (v3_out[0] / v3_out[3]) + viewportXOffset);
		screenPositions[5] = (-viewportYScale * (v3_out[1] / v3_out[3]) + viewportYOffset);

		PixelEmitter emitter(this, screenPositions);

		rasterizer->RasterizeTriangle(screenPositions, &emitter);
	}

	return 0;
}

bool Pipeline::ZTest(int x, int y, float z)
{
	if (z < depthBuffer[x + y * bufferWidth])
	{
		depthBuffer[x + y * bufferWidth] = z;
		return true;
	}
	return false;
}
