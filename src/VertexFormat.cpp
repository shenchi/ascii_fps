#include "VertexFormat.h"

const VertexFormat::member_pointer VertexFormat::offsets[VertexWeights + 1] = {
	nullptr,
	&VertexFormat::offsetPosition,
	&VertexFormat::offsetNormal,
	nullptr,
	&VertexFormat::offsetColor,
	nullptr,
	nullptr,
	nullptr,
	&VertexFormat::offsetUV,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	&VertexFormat::offsetWeights,
};

VertexFormat::VertexFormat(unsigned int attribs)
	:
	attribute(attribs),
	stride(0),
	offsetPosition(0),
	offsetNormal(-1),
	offsetColor(-1),
	offsetUV(-1),
	offsetWeights(-1)
{
	stride = VertexPositionSize +
		(Has(VertexNormal) * VertexNormalSize) +
		(Has(VertexColor) * VertexColorSize) +
		(Has(VertexUV) * VertexUVSize) +
		(Has(VertexWeights)* VertexWeightsSize);

	unsigned char pos = VertexPositionSize;

	if (Has(VertexNormal))
	{
		offsetNormal = pos;
		pos += VertexNormalSize;
	}
	if (Has(VertexColor))
	{
		offsetColor = pos;
		pos += VertexColorSize;
	}
	if (Has(VertexUV))
	{
		offsetUV = pos;
		pos += VertexUVSize;
	}
	if (Has(VertexWeights))
	{
		offsetWeights = pos;
	}
}
