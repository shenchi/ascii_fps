#pragma once

class PixelEmitter;

class Rasterizer
{
public:
	Rasterizer(int bufferWidth, int bufferHeight);
	~Rasterizer();

	void RasterizeTriangle(const float positions[6], PixelEmitter* interpolator);

	int GetBufferWidth() const;
	int GetBufferHeight() const;

private:

	int bufferWidth;
	int bufferHeight;
};

