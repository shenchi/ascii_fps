#pragma once

class ConsoleWindow
{
public:
	ConsoleWindow();

	int		Create(const wchar_t* title, short bufferWidth, short bufferHeight, short fontWidth, short fontHeight);
	int		Destroy();

	int		SetTitle(const wchar_t* title);
	int		SetTitleA(const char* title);

	int		Update();

	int		Clear(unsigned int color);

	int		SwapBuffers();
	int		Flush();

	void	SetColor(short x, short y, unsigned int color);

	bool	IsKeyDown(unsigned char vkCode) const;

	short	GetBufferWidth() const;
	short	GetBufferHeight() const;

private:
	struct ConsoleWindowDatas;

	short	fontWidth;
	short	fontHeight;

	short	bufferWidth;
	short	bufferHeight;

	ConsoleWindowDatas* datas;
	size_t	iCurrentBuffer = 0;
};

