#pragma once

class IColorBufferAdaptor;
class Shader;

class PixelEmitter
{
public:
	PixelEmitter(const float* screenPosisions, float* pixelDataBlock, const Shader* pixelShader, IColorBufferAdaptor* adaptor);

	void EmitPixel(int x, int y);

private:
	const float*			screenPosisions;
	float*					pixelDataBlock;
	const Shader*			pixelShader;
	size_t					stride;
	IColorBufferAdaptor*	adaptor;
	float					outputColor[4];

	float					det;
	float					y2_3, x3_2, y3_1, x1_3;

};

