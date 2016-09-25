#include "Rasterizer.h"
#include "PixelEmitter.h"

Rasterizer::Rasterizer(int bufferWidth, int bufferHeight)
	:
	bufferWidth(bufferWidth),
	bufferHeight(bufferHeight)
{
}

Rasterizer::~Rasterizer()
{
}

#define min(a, b, c) (a < b ? (a < c ? a : c) : (b < c ? b : c))
#define max(a, b, c) (a > b ? (a > c ? a : c) : (b > c ? b : c))

/*
Rasterizing with "half-space" method:
http://forum.devmaster.net/t/advanced-rasterization/6145
*/
void Rasterizer::RasterizeTriangle(const float positions[6], PixelEmitter* emitter)
{
	int x1 = (int)(positions[0]);
	int y1 = (int)(positions[1]);
	int x2 = (int)(positions[2]);
	int y2 = (int)(positions[3]);
	int x3 = (int)(positions[4]);
	int y3 = (int)(positions[5]);

	int dx1_2 = x1 - x2;
	int dx2_3 = x2 - x3;
	int dx3_1 = x3 - x1;

	int dy1_2 = y1 - y2;
	int dy2_3 = y2 - y3;
	int dy3_1 = y3 - y1;

	int c1 = x1 * y2 - x2 * y1;
	int c2 = x2 * y3 - x3 * y2;
	int c3 = x3 * y1 - x1 * y3;

	if (dy1_2 > 0 || (dy1_2 == 0 && dx1_2 < 0)) c1++;
	if (dy2_3 > 0 || (dy2_3 == 0 && dx2_3 < 0)) c2++;
	if (dy3_1 > 0 || (dy3_1 == 0 && dx3_1 < 0)) c3++;

	int minX = min(x1, x2, x3);
	int minY = min(y1, y2, y3);
	int maxX = max(x1, x2, x3);
	int maxY = max(y1, y2, y3);

	int f1_min = dy1_2 * minX - dx1_2 * minY + c1;
	int f2_min = dy2_3 * minX - dx2_3 * minY + c2;
	int f3_min = dy3_1 * minX - dx3_1 * minY + c3;

	for (int y = minY; y <= maxY; ++y)
	{
		int f1 = f1_min;
		int f2 = f2_min;
		int f3 = f3_min;

		for (int x = minX; x <= maxX; ++x)
		{
			if (f1 > 0 && f2 > 0 && f3 > 0 && x >= 0 && x < bufferWidth && y >= 0 && y < bufferHeight)
			{
				emitter->EmitPixel(x, y);
			}

			f1 += dy1_2;
			f2 += dy2_3;
			f3 += dy3_1;
		}

		f1_min -= dx1_2;
		f2_min -= dx2_3;
		f3_min -= dx3_1;
	}
}

int Rasterizer::GetBufferWidth() const
{
	return bufferWidth;
}

int Rasterizer::GetBufferHeight() const
{
	return bufferHeight;
}
