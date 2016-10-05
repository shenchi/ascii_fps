#pragma once

#include "ColorBufferAdaptor.h"

class ConsoleWindow;

class ConsoleWindowAdaptor : public IColorBufferAdaptor
{
public:
	ConsoleWindowAdaptor(ConsoleWindow* window) : window(window) {}

	virtual void WriteRenderTarget(int x, int y, const float* color);

	virtual int GetBufferWidth() const;

	virtual int GetBufferHeight() const;

	virtual void ClearColorBuffer(const float* color);

private:
	ConsoleWindow* window;
};
