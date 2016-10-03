#pragma once

class PixelEmitter;

class Rasterizer
{
public:
	Rasterizer(int bufferWidth, int bufferHeight);
	~Rasterizer();

	void RasterizeTriangle(const float* positions, PixelEmitter* emitter);
	void RasterizeTriangle(int x1, int y1, int x2, int y2, int x3, int y3, PixelEmitter* emitter);

	int GetBufferWidth() const;
	int GetBufferHeight() const;

private:

	int bufferWidth;
	int bufferHeight;
};

