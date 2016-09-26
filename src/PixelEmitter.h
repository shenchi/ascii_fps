#pragma once

class IColorBufferAdaptor;
class Pipeline;
class Shader;
class DepthControl;

class PixelEmitter
{
public:
	PixelEmitter(Pipeline* pipeline, const float* screenPosisions);

	void EmitPixel(int x, int y);

private:
	Pipeline*				pipeline;
	const float*			screenPosisions;
	size_t					stride;
	float					outputColor[4];

	float					det;
	float					y2_3, x3_2, y3_1, x1_3;

};

