#include "MouseInputReceiver.h"
#include <Windows.h>
#include "WinAPIDebug.h"

namespace // we don't want explose these to outside
{
	const wchar_t* pClassName = L"MouseInputReceiver";
	const size_t inputBufferSize = 512;

	HINSTANCE	hInstance = nullptr;
	HWND		hWnd = nullptr;

	int*		pMousePositionX = nullptr;
	int*		pMousePositionY = nullptr;
	int*		pMousePositionDeltaX = nullptr;
	int*		pMousePositionDeltaY = nullptr;

	LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_INPUT:
		{
			UINT dwSize = inputBufferSize;
			static BYTE lpb[inputBufferSize];

			if (-1 == GetRawInputData((HRAWINPUT)lParam, RID_INPUT,
				lpb, &dwSize, sizeof(RAWINPUTHEADER)))
			{
				DWORD ret = GetLastError();
				return DefWindowProc(hWnd, uMsg, wParam, lParam);
			}

			RAWINPUT* raw = (RAWINPUT*)lpb;

			if (raw->header.dwType == RIM_TYPEMOUSE)
			{
				*pMousePositionDeltaX += raw->data.mouse.lLastX;
				*pMousePositionDeltaY += raw->data.mouse.lLastY;
				*pMousePositionX += raw->data.mouse.lLastX;
				*pMousePositionY += raw->data.mouse.lLastY;
			}
			break;
		}
		default:
			break;
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

int MouseInputReceiver::Start(int* dataBlock)
{
	if (nullptr != hInstance)
	{
		return -1;
	}

	hInstance = GetModuleHandle(nullptr);

	WNDCLASS wndCls = {};
	wndCls.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	wndCls.hInstance = hInstance;
	wndCls.lpfnWndProc = WndProc;
	wndCls.lpszClassName = pClassName;
	wndCls.style = CS_NOCLOSE;

	CHECK(RegisterClass(&wndCls));
	CHECK(hWnd = CreateWindow(pClassName, L"", 0 /*WS_BORDER*/, CW_USEDEFAULT, CW_USEDEFAULT, 1, 1, nullptr, nullptr, hInstance, nullptr));
	ShowWindow(hWnd, SW_HIDE);

	RAWINPUTDEVICE rid{};
	rid.usUsagePage = 1;
	rid.usUsage = 2;
	rid.hwndTarget = hWnd;
	rid.dwFlags = RIDEV_INPUTSINK;// | RIDEV_NOLEGACY;

	CHECK(RegisterRawInputDevices(&rid, 1, sizeof(rid)));

	pMousePositionX = dataBlock;
	pMousePositionY = dataBlock + 1;
	pMousePositionDeltaX = dataBlock + 2;
	pMousePositionDeltaY = dataBlock + 3;

	return 0;
}

void MouseInputReceiver::Update()
{
	if (nullptr == hWnd)
		return;

	*pMousePositionDeltaX = 0;
	*pMousePositionDeltaY = 0;

	MSG msg;
	while (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void MouseInputReceiver::Stop()
{
	if (nullptr == hWnd)
		return;

	DestroyWindow(hWnd);
	hWnd = nullptr;

	UnregisterClass(pClassName, hInstance);
	hInstance = nullptr;
}
