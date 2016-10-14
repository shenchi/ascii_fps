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

	short rgb_table[] = {
		/*0 0 0*/	0b0000,
		/*0 0 1*/	0b0001,
		/*0 0 2*/	0b1001,
		/*0 0 3*/	0,
		/*0 1 0*/	0b0010,
		/*0 1 1*/	0b0011,
		/*0 1 2*/	0b1001,	//*
		/*0 1 3*/	0,
		/*0 2 0*/	0b1010,
		/*0 2 1*/	0b1010, //*
		/*0 2 2*/	0b1011,
		/*0 2 3*/	0,
		/*0 3 0*/	0,
		/*0 3 1*/	0,
		/*0 3 2*/	0,
		/*0 3 3*/	0,
		/*1 0 0*/	0b0100,
		/*1 0 1*/	0b0101,
		/*1 0 2*/	0b1001,	//*
		/*1 0 3*/	0,
		/*1 1 0*/	0b0110,
		/*1 1 1*/	0b0111,
		/*1 1 2*/	0b1001, //*
		/*1 1 3*/	0,
		/*1 2 0*/	0b1010, //*
		/*1 2 1*/	0b1010, //*
		/*1 2 2*/	0b1011, //*
		/*1 2 3*/	0,
		/*1 3 0*/	0,
		/*1 3 1*/	0,
		/*1 3 2*/	0,
		/*1 3 3*/	0,
		/*2 0 0*/	0b1100,
		/*2 0 1*/	0b1100, //*
		/*2 0 2*/	0b1101,
		/*2 0 3*/	0,
		/*2 1 0*/	0b1100, //*
		/*2 1 1*/	0b1100, //*
		/*2 1 2*/	0b1101, //*
		/*2 1 3*/	0,
		/*2 2 0*/	0b1110,
		/*2 2 1*/	0b1110, //*
		/*2 2 2*/	0b1111,
	};

	void ConvertRGBToAscii(const float* color, char & ascii, unsigned short & attr)
	{
		/*attr = 8;
		if (color[0] > 0.5f) attr |= 4;
		if (color[1] > 0.5f) attr |= 2;
		if (color[2] > 0.5f) attr |= 1;*/
		int r = static_cast<int>(color[0] * 2.0f + 0.5f);
		int g = static_cast<int>(color[1] * 2.0f + 0.5f);
		int b = static_cast<int>(color[2] * 2.0f + 0.5f);
		int rgb = (r << 4) | (g << 2) | b;
		attr = rgb_table[rgb];

		float value = color[0] > color[1] ? (color[0] > color[2] ? color[0] : color[2]) : (color[1] > color[2] ? color[1] : color[2]);
		ascii = ConvertValueToAscii(value);
	}
}