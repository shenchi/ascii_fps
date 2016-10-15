#pragma once

class ConsoleWindow;

class ConsoleWindowPrinter
{
public:
	ConsoleWindowPrinter(ConsoleWindow* window);

	void			Print(int x, int y, char ch, const float* color);
	void			Print(int x, int y, const char* string, const float* color);

	inline int		FontWidth() { return 8; }
	int				FontHeight();

private:
	ConsoleWindow*	window;
	int				bufferWidth;
	int				bufferHeight;
};
