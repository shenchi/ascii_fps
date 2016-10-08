#include "ConsoleWindow.h"
#include <Windows.h>
#include "WinAPIDebug.h"
#include "MouseInputReceiver.h"

static const int BUFFER_COUNT = 2;

struct ConsoleWindowDatas
{
	HWND			hWnd;
	HANDLE			hStdout;
	HANDLE			hStdin;
	HANDLE			hBuffers[BUFFER_COUNT];
	HANDLE			hBuffer;
	SMALL_RECT		screenRect;
	CHAR_INFO*		buffers[BUFFER_COUNT];
	CHAR_INFO*		buffer;
	WNDPROC			pWndProc;
	int				mousePosX;
	int				mousePosY;
	int				mousePosDeltaX;
	int				mousePosDeltaY;
};

ConsoleWindow::ConsoleWindow()
	:
	datas(nullptr)
{
}

int ConsoleWindow::Create(const wchar_t* title, short _bufferWidth, short _bufferHeight, short _fontWidth, short _fontHeight)
{
	if (nullptr != datas)
	{
		return -1;
	}

	datas = new ConsoleWindowDatas{};

	bufferWidth = _bufferWidth;
	bufferHeight = _bufferHeight;
	fontWidth = _fontWidth;
	fontHeight = _fontHeight;

	CHECK(AllocConsole());

	datas->hWnd = GetConsoleWindow();

	SetWindowLong(datas->hWnd, GWL_STYLE, GetWindowLong(datas->hWnd, GWL_STYLE) & ~(WS_SIZEBOX | WS_MAXIMIZEBOX));

	datas->hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	datas->hStdin = GetStdHandle(STD_INPUT_HANDLE);

	{
		DWORD mode;
		CHECK(GetConsoleMode(datas->hStdout, &mode));
		mode = mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
		CHECK(SetConsoleMode(datas->hStdout, mode));

		CONSOLE_FONT_INFOEX fontInfo = {};
		fontInfo.cbSize = sizeof(fontInfo);
		CHECK(GetCurrentConsoleFontEx(datas->hStdout, FALSE, &fontInfo));
		fontInfo.nFont = 0;
		fontInfo.FontFamily = 54;
		fontInfo.FontWeight = 400;
		lstrcpyW(fontInfo.FaceName, L"Consolas");
		fontInfo.dwFontSize = { fontWidth, fontHeight };
		CHECK(SetCurrentConsoleFontEx(datas->hStdout, FALSE, &fontInfo));

		CHECK(SetConsoleScreenBufferSize(datas->hStdout, { bufferWidth, bufferHeight }));
		datas->screenRect = { 0, 0, bufferWidth - 1, bufferHeight - 1 };
		CHECK(SetConsoleWindowInfo(datas->hStdout, TRUE, &(datas->screenRect)));

		CONSOLE_CURSOR_INFO cursorInfo = {};
		CHECK(GetConsoleCursorInfo(datas->hStdout, &cursorInfo));
		cursorInfo.bVisible = FALSE;
		CHECK(SetConsoleCursorInfo(datas->hStdout, &cursorInfo));

		CHECK(SetConsoleTitle(title));
	}

	{
		datas->hBuffers[0] = datas->hStdout;
		datas->buffers[0] = new CHAR_INFO[bufferWidth * bufferHeight]{};

		for (size_t i = 1; i < BUFFER_COUNT; ++i)
		{
			datas->hBuffers[i] = CreateConsoleScreenBuffer(GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, CONSOLE_TEXTMODE_BUFFER, nullptr);
			CHECK(SetConsoleScreenBufferSize(datas->hBuffers[i], { bufferWidth, bufferHeight }));

			datas->buffers[i] = new CHAR_INFO[bufferWidth * bufferHeight]{};
		}

		datas->buffer = datas->buffers[0];
		datas->hBuffer = datas->hBuffers[0];
	}

	{
		CHAR_INFO ci = {};
		ci.Char.AsciiChar = ' ';
		ci.Attributes = 0;

		for (size_t i = 0; i < BUFFER_COUNT; i++)
		{
			for (short y = 0; y < bufferHeight; ++y)
			{
				for (short x = 0; x < bufferWidth; ++x)
				{
					datas->buffers[i][y * bufferWidth + x] = ci;
				}
			}

			SMALL_RECT r = datas->screenRect;
			CHECK(WriteConsoleOutputA(datas->hBuffers[i], datas->buffers[i], { bufferWidth, bufferHeight }, { 0, 0 }, &r));
		}
	}

	{
		RECT windowRect;
		RECT desktopRect;
		GetWindowRect(datas->hWnd, &windowRect);
		GetClientRect(GetDesktopWindow(), &desktopRect);
		int winWidth = windowRect.right - windowRect.left;
		int winHeight = windowRect.bottom - windowRect.top;
		int screenWidth = desktopRect.right - desktopRect.left;
		int screenHeight = desktopRect.bottom - desktopRect.top;
		int winX = (screenWidth - winWidth) / 2;
		int winY = (screenHeight - winHeight) / 2;
		SetWindowPos(datas->hWnd, nullptr, winX, winY, 0, 0, SWP_NOSIZE);

		desktopRect.left = desktopRect.right;
		desktopRect.top = desktopRect.bottom;
		ClipCursor(&desktopRect);
	}

	MouseInputReceiver::Start(&(datas->mousePosX));

	return 0;
}

int ConsoleWindow::Destroy()
{
	if (nullptr == datas)
	{
		return -1;
	}

	ClipCursor(nullptr);

	MouseInputReceiver::Stop();

	CHECK(SetConsoleActiveScreenBuffer(datas->hStdout));
	for (size_t i = 1; i < BUFFER_COUNT; ++i)
	{
		CHECK(CloseHandle(datas->hBuffers[i]));

		delete[] datas->buffers[i];
	}
	CHECK(FreeConsole());

	delete datas;

	return 0;
}

int ConsoleWindow::SetTitle(const wchar_t * title)
{
	CHECK(SetConsoleTitle(title));
	return 0;
}

int ConsoleWindow::SetTitleA(const char * title)
{
	CHECK(SetConsoleTitleA(title));
	return 0;
}

int ConsoleWindow::Update()
{
	MouseInputReceiver::Update();

	return 0;
}

int ConsoleWindow::Clear(unsigned short color, char ascii)
{
	if (nullptr == datas)
	{
		return -1;
	}

	for (short y = 0; y < bufferHeight; ++y)
	{
		for (short x = 0; x < bufferWidth; ++x)
		{
			CHAR_INFO& info = datas->buffer[y * bufferWidth + x];
			info.Char.AsciiChar = ascii;
			info.Attributes = color;
		}
	}

	return 0;
}

int ConsoleWindow::SwapBuffers()
{
	if (nullptr == datas)
	{
		return -1;
	}

	if (BUFFER_COUNT > 1)
	{
		size_t idx = iCurrentBuffer;
		iCurrentBuffer = (iCurrentBuffer + 1) % BUFFER_COUNT;
		datas->buffer = datas->buffers[iCurrentBuffer];
		datas->hBuffer = datas->hBuffers[iCurrentBuffer];

		CHECK(SetConsoleActiveScreenBuffer(datas->hBuffers[idx]));
	}

	return 0;
}

int ConsoleWindow::Flush()
{
	if (nullptr == datas)
	{
		return -1;
	}

	{
		SMALL_RECT r = datas->screenRect;
		CHECK(WriteConsoleOutputA(datas->hBuffer, datas->buffer, { bufferWidth, bufferHeight }, { 0, 0 }, &r));
	}

	return 0;
}

void ConsoleWindow::SetColor(short x, short y, unsigned int color, char ascii)
{
	CHAR_INFO& info = datas->buffer[y * bufferWidth + x];
	info.Char.AsciiChar = ascii;
	info.Attributes = color;
}

bool ConsoleWindow::IsKeyDown(unsigned char vkCode) const
{
	return ((GetAsyncKeyState(vkCode) & 0x8000) != 0);
}

void ConsoleWindow::GetMousePosition(int & x, int & y) const
{
	x = datas->mousePosX;
	y = datas->mousePosY;
}

int ConsoleWindow::GetMousePositionX() const
{
	return datas->mousePosX;
}

int ConsoleWindow::GetMousePositionY() const
{
	return datas->mousePosY;
}

void ConsoleWindow::GetMouseDeltaPosition(int & x, int & y) const
{
	x = datas->mousePosDeltaX;
	y = datas->mousePosDeltaY;
}

int ConsoleWindow::GetMousePositionDeltaX() const
{
	return datas->mousePosDeltaX;
}

int ConsoleWindow::GetMousePositionDeltaY() const
{
	return datas->mousePosDeltaY;
}

short ConsoleWindow::GetBufferWidth() const
{
	return bufferWidth;
}

short ConsoleWindow::GetBufferHeight() const
{
	return bufferHeight;
}
