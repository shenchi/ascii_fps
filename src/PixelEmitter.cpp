#include "PixelEmitter.h"
#include "Pipeline.h"
#include "ColorBufferAdaptor.h"
#include "Shader.h"

#define x1 screenPosisions[0]
#define y1 screenPosisions[1]
#define x2 screenPosisions[2]
#define y2 screenPosisions[3]
#define x3 screenPosisions[4]
#define y3 screenPosisions[5]

PixelEmitter::PixelEmitter(Pipeline* pipeline, const float* screenPosisions, const float* v1_data, const float* v2_data, const float* v3_data)
	:
	pipeline(pipeline),
	screenPosisions(screenPosisions),
	p1(v1_data),
	p2(v2_data),
	p3(v3_data),
	data(pipeline->interpolateDataBlock),
	stride(pipeline->pixelShader->Stride())
{
	y2_3 = y2 - y3;
	x3_2 = x3 - x2;
	y3_1 = y3 - y1;
	x1_3 = x1 - x3;

	det = y2_3 * x1_3 - x3_2 * y3_1;
}

void PixelEmitter::EmitPixel(int x, int y)
{
	float a = (y2_3 * (x - x3) + x3_2 * (y - y3)) / det;
	float b = (y3_1 * (x - x3) + x1_3 * (y - y3)) / det;
	float c = 1.0f - a - b;

	for (size_t i = 0; i < 4; ++i)
	{
		data[i] = a * p1[i] + b * p2[i] + c * p3[i];
	}

	float z = data[2] / data[3] * 0.5f + 0.5f;
	if (!pipeline->ZTest(x, y, z))
	{
		return;
	}

	for (size_t i = 4; i < stride; ++i)
	{
		data[i] = a * p1[i] + b * p2[i] + c * p3[i];
	}

	pipeline->pixelShader->Main(data, outputColor);
	pipeline->colorBufferAdaptor->WriteRenderTarget(x, y, outputColor);
}
