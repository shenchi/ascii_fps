#include "ConsoleWindowPrinter.h"

#include "ConsoleWindow.h"
#include "AsciiGrayScale.h"

//#define HALF_HEIGHT_MODE

#define I_KNOW_I_AM_INCLUDING_FONT
#include "font.h"

#ifdef HALF_HEIGHT_MODE
constexpr int fontHeight = 4;
#else
constexpr int fontHeight = 8;
#endif

ConsoleWindowPrinter::ConsoleWindowPrinter(ConsoleWindow * window)
	:
	window(window)
{
	bufferWidth = static_cast<int>(window->GetBufferWidth());
	bufferHeight = static_cast<int>(window->GetBufferHeight());
}

void ConsoleWindowPrinter::Print(int x, int y, char ch, const float* color)
{
	if (x < 0 || x >= bufferWidth || y < 0 || y >= bufferHeight || ch < 0)
		return;

	const char *p = bmFont + (ch * fontHeight * 8);

	for (int j = 0; j < fontHeight; j++)
	{
		for (int i = 0; i < 8; i++)
		{
			unsigned short attr = 0xf;
			char code = *(p++);
			int sx = x + i, sy = y + j;
			if (sx >= bufferWidth || sy >= bufferHeight)
				continue;
			if (code == ' ')
				continue;

			if (nullptr != color)
			{
				AsciiGrayScale::ConvertRGBToAscii(color, code, attr);
			}

			window->SetColor(sx, sy, attr, code);
		}
	}
}

void ConsoleWindowPrinter::Print(int x, int y, const char* string, const float* color)
{
	if (x < 0 || x >= bufferWidth || y < 0 || y >= bufferHeight)
		return;

	const char* p = string;
	while (*p != 0)
	{
		Print(x, y, *p, color);
		x += 8;
		++p;
	}
}

int ConsoleWindowPrinter::FontHeight()
{
	return fontHeight;
}
