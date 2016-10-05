#include "ConsoleWindowAdaptor.h"
#include "ConsoleWindow.h"
#include "AsciiGrayScale.h"

void ConsoleWindowAdaptor::WriteRenderTarget(int x, int y, const float * color)
{
	unsigned short col;
	char ascii;
	AsciiGrayScale::ConvertRGBToAscii(color, ascii, col);
	window->SetColor(static_cast<short>(x), static_cast<short>(y), col, ascii);
}

int ConsoleWindowAdaptor::GetBufferWidth() const
{
	return window->GetBufferWidth();
}

int ConsoleWindowAdaptor::GetBufferHeight() const
{
	return window->GetBufferHeight();
}

void ConsoleWindowAdaptor::ClearColorBuffer(const float * color)
{
	unsigned short col = 0;
	if (color[0] > 0.5f) col |= 4;
	if (color[1] > 0.5f) col |= 2;
	if (color[2] > 0.5f) col |= 1;
	window->Clear(col);
}
