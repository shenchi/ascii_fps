#include "AsciiGrayScale.h"

namespace AsciiGrayScale
{
	// grayscale table for value from 0 to 1
	// double the last char
	//const char valueTable[] = " .^W$\xb1\xb2\xdb\xdb";
	const char valueTable[] = " .^W$@";
	const int valueCount = sizeof(valueTable) - 2;

	char ConvertValueToAscii(float value)
	{
		size_t index = (size_t)((value - 0.000001f) * valueCount);
		return valueTable[index];
	}

	void ConvertRGBToAscii(const float* color, char & ascii, unsigned short & attr)
	{
		attr = 8;
		if (color[0] > 0.5f) attr |= 4;
		if (color[1] > 0.5f) attr |= 2;
		if (color[2] > 0.5f) attr |= 1;
		float value = color[0] > color[1] ? (color[0] > color[2] ? color[0] : color[2]) : (color[1] > color[2] ? color[1] : color[2]);
		ascii = ConvertValueToAscii(value);
	}
}