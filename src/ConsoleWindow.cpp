#include "ConsoleWindow.h"
#include <Windows.h>

static const int BUFFER_COUNT = 2;

#ifdef _DEBUG
#define STRINGIFY(x) #x
#define FILE_AND_LINE(x) __FILE__ ":" STRINGIFY(x)
#define CHECK(x) if (!(x)) { int ret = GetLastError(); MessageBoxA(nullptr, FILE_AND_LINE(__LINE__), "Error", MB_OK); return ret; }
#else
#define CHECK(x) if (!(x)) { return GetLastError(); }
#endif

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
};

namespace
{
	ConsoleWindowDatas* gWndDatas = nullptr;

	LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_INPUT:
		{
			UINT dwSize = 40;
			static BYTE lpb[40];

			GetRawInputData((HRAWINPUT)lParam, RID_INPUT,
				lpb, &dwSize, sizeof(RAWINPUTHEADER));

			RAWINPUT* raw = (RAWINPUT*)lpb;

			if (raw->header.dwType == RIM_TYPEMOUSE)
			{
				gWndDatas->mousePosX += raw->data.mouse.lLastX;
				gWndDatas->mousePosY += raw->data.mouse.lLastY;
			}
			break;
		}
		default:
			break;
		}
		return gWndDatas->pWndProc(hWnd, uMsg, wParam, lParam);
	}
}

ConsoleWindow::ConsoleWindow()
	:
	datas(nullptr)
{
}

int ConsoleWindow::Create(const wchar_t* title, short _bufferWidth, short _bufferHeight, short _fontWidth, short _fontHeight)
{
	if (nullptr != datas || nullptr != gWndDatas)
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
		ci.Char.AsciiChar = char(219);
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
		gWndDatas = datas;
		datas->pWndProc = (WNDPROC)GetWindowLongPtr(datas->hWnd, GWLP_WNDPROC);
		SetWindowLongPtr(datas->hWnd, GWLP_WNDPROC, (LONG_PTR)WndProc);

		RAWINPUTDEVICE rawInputDevice = {};
		rawInputDevice.usUsagePage = 1;
		rawInputDevice.usUsage = 2;
		rawInputDevice.hwndTarget = datas->hWnd;
		rawInputDevice.dwFlags = RIDEV_INPUTSINK;// RIDEV_CAPTUREMOUSE;
		CHECK(RegisterRawInputDevices(&rawInputDevice, 1, sizeof(RAWINPUTDEVICE)));
		DWORD ret = GetLastError();
	}

	return 0;
}

int ConsoleWindow::Destroy()
{
	if (nullptr == datas)
	{
		return -1;
	}

	CHECK(SetConsoleActiveScreenBuffer(datas->hStdout));
	for (size_t i = 1; i < BUFFER_COUNT; ++i)
	{
		CHECK(CloseHandle(datas->hBuffers[i]));

		delete[] datas->buffers[i];
	}
	CHECK(FreeConsole());

	delete datas;

	gWndDatas = nullptr;

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
	// nothing to do right now
	// we use GetAsyncKeyState() instead of GetKeyboardState()

	/*if (nullptr == datas)
	{
		return -1;
	}

	CHECK(GetKeyboardState(datas->keyStates));*/

	return 0;
}

int ConsoleWindow::Clear(unsigned short color)
{
	if (nullptr == datas)
	{
		return -1;
	}

	for (short y = 0; y < bufferHeight; ++y)
	{
		for (short x = 0; x < bufferWidth; ++x)
		{
			datas->buffer[y * bufferWidth + x].Attributes = color;
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

void ConsoleWindow::SetColor(short x, short y, unsigned int color)
{
	datas->buffer[y * bufferWidth + x].Attributes = color;
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

short ConsoleWindow::GetBufferWidth() const
{
	return bufferWidth;
}

short ConsoleWindow::GetBufferHeight() const
{
	return bufferHeight;
}
