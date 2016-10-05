#pragma once

class IColorBufferAdaptor
{
public:
	virtual void WriteRenderTarget(int x, int y, const float* color) = 0;
	virtual int GetBufferWidth() const = 0;
	virtual int GetBufferHeight() const = 0;
	virtual void ClearColorBuffer(const float* color) = 0;
};