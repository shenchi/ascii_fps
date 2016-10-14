#include "ConsoleWindowPrinter.h"

#include "ConsoleWindow.h"

#define I_KNOW_I_AM_INCLUDING_FONT
#include "font.h"


ConsoleWindowPrinter::ConsoleWindowPrinter(ConsoleWindow * window)
	:
	window(window)
{
	bufferWidth = static_cast<int>(window->GetBufferWidth());
	bufferHeight = static_cast<int>(window->GetBufferHeight());
}

void ConsoleWindowPrinter::Print(int x, int y, char ch, int attribute, bool half)
{
	if (x < 0 || x >= bufferWidth || y < 0 || y >= bufferHeight || ch < 0)
		return;

	const int height = (half ? 4 : 8);
	const char *p = (half ? bmFont_Half : bmFont) + (ch * height * 8);

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < 8; i++)
		{
			char code = *(p++);
			int sx = x + i, sy = y + j;
			if (sx >= bufferWidth || sy >= bufferHeight)
				continue;
			if (code == ' ')
				continue;
			window->SetColor(sx, sy, attribute, code);
		}
	}
}

void ConsoleWindowPrinter::Print(int x, int y, const char* string, int attribute)
{
	if (x < 0 || x >= bufferWidth || y < 0 || y >= bufferHeight)
		return;

	const char* p = string;
	while (*p != 0)
	{
		Print(x, y, *p, attribute, false);
		x += 8;
		++p;
	}
}
