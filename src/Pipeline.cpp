#include "Pipeline.h"
#include "Shader.h"
#include "Rasterizer.h"
#include "PixelEmitter.h"

#ifdef _DEBUG
#include <cassert>
#endif

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
	pixelDataBlock(nullptr),
	clippedDataBlock(nullptr),
	interpolateDataBlock(nullptr)
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

	if (nullptr != clippedDataBlock)
	{
		delete[] clippedDataBlock;
	}

	if (nullptr != interpolateDataBlock)
	{
		delete[] interpolateDataBlock;
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

	// TODO: multithreaded: data blocks below located in thread-local memory

	if (nullptr != pixelDataBlock)
	{
		delete[] pixelDataBlock;
	}
	pixelDataBlock = new float[pixelDataStride * 3];

	if (nullptr != clippedDataBlock)
	{
		delete[] clippedDataBlock;
	}
	clippedDataBlock = new float[pixelDataStride * 80];

	if (nullptr != interpolateDataBlock)
	{
		delete[] interpolateDataBlock;
	}

	interpolateDataBlock = new float[pixelDataStride];
}

void Pipeline::SetConstantBuffer(const float * buffer)
{
	vertexShader->SetConstantBuffer(buffer);
	pixelShader->SetConstantBuffer(buffer);
}

static inline bool IsCW(float* v1, float* v2, float* v3)
{
	float dx2_1 = v2[0] - v1[0];
	float dy2_1 = v2[1] - v1[1];
	float dx3_1 = v3[0] - v1[0];
	float dy3_1 = v3[1] - v1[1];
	return (dx2_1 * dy3_1 - dy2_1 * dx3_1 > 0);
}

int Pipeline::Draw(const float* vertices, size_t numVertices, const int* indices, size_t numIndices)
{
	if (numIndices % 3 != 0)
	{
		return -1;
	}

	float* v1_out = pixelDataBlock;
	float* v2_out = v1_out + pixelDataStride;
	float* v3_out = v2_out + pixelDataStride;

	size_t clippedCount = 0;

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

		// clipping
		Clip(pixelDataBlock, 3, clippedDataBlock, clippedCount, pixelDataStride);

		if (clippedCount < 3) // less than 3 vertcies
		{
			continue;
		}

		// rasterization
		for (size_t t = 1; t < clippedCount - 1; ++t)
		{
			// 0, t, t + 1
			const float* v1_interp = clippedDataBlock;
			const float* v2_interp = clippedDataBlock + t * pixelDataStride;
			const float* v3_interp = clippedDataBlock + (t + 1) * pixelDataStride;

			// viewport transform
			screenPositions[0] = (viewportXScale * (v1_interp[0] / v1_interp[3]) + viewportXOffset);
			screenPositions[1] = (-viewportYScale * (v1_interp[1] / v1_interp[3]) + viewportYOffset);
			screenPositions[2] = (viewportXScale * (v2_interp[0] / v2_interp[3]) + viewportXOffset);
			screenPositions[3] = (-viewportYScale * (v2_interp[1] / v2_interp[3]) + viewportYOffset);
			screenPositions[4] = (viewportXScale * (v3_interp[0] / v3_interp[3]) + viewportXOffset);
			screenPositions[5] = (-viewportYScale * (v3_interp[1] / v3_interp[3]) + viewportYOffset);

			PixelEmitter emitter(this, screenPositions, v1_interp, v2_interp, v3_interp);

			rasterizer->RasterizeTriangle(screenPositions, &emitter);
		}

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

/*
* Based on Sutherland–Hodgman algorithm
* https://en.wikipedia.org/wiki/Sutherland%E2%80%93Hodgman_algorithm
* http://fabiensanglard.net/polygon_codec/
*/
void Pipeline::Clip(const float* inputList, size_t inputCount, float* outputList, size_t& outputCount, size_t stride) const
{
	float* const temp_buffer = outputList + 40 * stride;

	constexpr float W_CLIPPING_PLANE = 0.00001f;

#define CLIP_W(IN_BUFFER, OUT_BUFFER) \
	{\
		const float* in = IN_BUFFER;\
		float* out = OUT_BUFFER;\
		outputCount = 0;\
		size_t last_index = inputCount - 1;\
		for (size_t i = 0; i < inputCount; ++i)\
		{\
			size_t cur = i * stride;\
			size_t prv = last_index * stride;\
			if (in[cur + 3] > W_CLIPPING_PLANE)\
			{\
				if (!(in[prv + 3] > W_CLIPPING_PLANE))\
				{\
					float t = (W_CLIPPING_PLANE - in[prv + 3]) / (in[cur + 3] - in[prv + 3]);\
					size_t outp = (outputCount++) * stride;\
					out[outp + 3] = W_CLIPPING_PLANE;\
					out[outp] = in[prv] + t * (in[cur] - in[prv]);\
					out[outp + 1] = in[prv + 1] + t * (in[cur + 1] - in[prv + 1]);\
					out[outp + 2] = in[prv + 2] + t * (in[cur + 2] - in[prv + 2]);\
					for (size_t p = 4; p < stride; ++p) {\
						out[outp + p] = in[prv + p] + t * (in[cur + p] - in[prv + p]);\
					}\
				}\
				size_t outp = (outputCount++) * stride;\
				for (size_t p = 0; p < stride; ++p) {\
					out[outp++] = in[cur + p];\
				}\
			}\
			else if (in[prv + 3] > W_CLIPPING_PLANE)\
			{\
				float t = (W_CLIPPING_PLANE - in[prv + 3]) / (in[cur + 3] - in[prv + 3]);\
				size_t outp = (outputCount++) * stride;\
				out[outp + 3] = W_CLIPPING_PLANE;\
				out[outp] = in[prv] + t * (in[cur] - in[prv]);\
				out[outp + 1] = in[prv + 1] + t * (in[cur + 1] - in[prv + 1]);\
				out[outp + 2] = in[prv + 2] + t * (in[cur + 2] - in[prv + 2]);\
				for (size_t p = 4; p < stride; ++p) {\
					out[outp + p] = in[prv + p] + t * (in[cur + p] - in[prv + p]);\
				}\
			}\
			last_index = i;\
		}\
		inputCount = outputCount;\
	}

#define CLIP(OFFSET, CMP, SIGN, IN_BUFFER, OUT_BUFFER) \
	{\
		const float* in = IN_BUFFER;\
		float* out = OUT_BUFFER;\
		outputCount = 0;\
		size_t last_index = inputCount - 1;\
		for (size_t i = 0; i < inputCount; ++i)\
		{\
			size_t cur = i * stride;\
			size_t prv = last_index * stride;\
			if (in[cur OFFSET] CMP SIGN in[cur + 3])\
			{\
				if (!(in[prv OFFSET] CMP SIGN in[prv + 3]))\
				{\
					float t = (in[prv + 3] - (SIGN in[prv OFFSET])) / \
						(SIGN (in[cur OFFSET] - in[prv OFFSET]) - in[cur + 3] + in[prv + 3]);\
					size_t outp = (outputCount++) * stride;\
					for (size_t p = 0; p < stride; ++p) {\
						out[outp + p] = in[prv + p] + t * (in[cur + p] - in[prv + p]);\
					}\
				}\
				size_t outp = (outputCount++) * stride;\
				for (size_t p = 0; p < stride; ++p) {\
					out[outp++] = in[cur + p];\
				}\
			}\
			else if (in[prv OFFSET] CMP SIGN in[prv + 3])\
			{\
				float t = (in[prv + 3] - (SIGN in[prv OFFSET])) / \
					(SIGN (in[cur OFFSET] - in[prv OFFSET]) - in[cur + 3] + in[prv + 3]);\
				size_t outp = (outputCount++) * stride;\
				for (size_t p = 0; p < stride; ++p) {\
					out[outp + p] = in[prv + p] + t * (in[cur + p] - in[prv + p]);\
				}\
			}\
			last_index = i;\
		}\
		inputCount = outputCount;\
	}

	CLIP_W(inputList, outputList);						// w = 0.0001

	CLIP(/*+0*/, < , /*+*/, outputList, temp_buffer);   // w = x;
	CLIP(/*+0*/, > , -, temp_buffer, outputList);		// w = -x;
	CLIP(+1, < , /*+*/, outputList, temp_buffer);		// w = y;
	CLIP(+1, > , -, temp_buffer, outputList);			// w = -y;
	CLIP(+2, < , /*+*/, outputList, temp_buffer);		// w = z;
	CLIP(+2, > , -, temp_buffer, outputList);			// w = -z;


#ifdef _DEBUG
	assert(outputCount <= 40);
#endif

#undef CLIP_W
#undef CLIP
}
