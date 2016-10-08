#pragma once

struct ConsoleWindowDatas;

class ConsoleWindow
{
public:
	ConsoleWindow();

	int		Create(const wchar_t* title, short bufferWidth, short bufferHeight, short fontWidth, short fontHeight);
	int		Destroy();

	int		SetTitle(const wchar_t* title);
	int		SetTitleA(const char* title);

	int		Update();

	int		Clear(unsigned short color, char ascii = '\xdb');

	int		SwapBuffers();
	int		Flush();

	void	SetColor(short x, short y, unsigned int color, char ascii = '\xdb');

	bool	IsKeyDown(unsigned char vkCode) const;

	void	GetMousePosition(int& x, int& y) const;
	int		GetMousePositionX() const;
	int		GetMousePositionY() const;

	void	GetMouseDeltaPosition(int& x, int& y) const;
	int		GetMousePositionDeltaX() const;
	int		GetMousePositionDeltaY() const;

	short	GetBufferWidth() const;
	short	GetBufferHeight() const;

private:
	short	fontWidth;
	short	fontHeight;

	short	bufferWidth;
	short	bufferHeight;

	ConsoleWindowDatas* datas;
	size_t	iCurrentBuffer = 0;
};

