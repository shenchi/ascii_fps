#pragma once

class ConsoleWindow;

class ConsoleWindowPrinter
{
public:
	ConsoleWindowPrinter(ConsoleWindow* window);

	void Print(int x, int y, char ch, int attribute, bool half);

	void Print(int x, int y, const char* string, int attribute);

private:
	ConsoleWindow*	window;
	int				bufferWidth;
	int				bufferHeight;
};
