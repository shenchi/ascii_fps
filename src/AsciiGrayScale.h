#pragma once

namespace AsciiGrayScale
{
	char ConvertValueToAscii(float value);

	void ConvertRGBToAscii(const float* color, char& ascii, unsigned short& attr);
}