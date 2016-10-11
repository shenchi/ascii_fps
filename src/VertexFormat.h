#pragma once

constexpr unsigned int VertexPosition = 1;
constexpr unsigned int VertexNormal = 2;
constexpr unsigned int VertexColor = 4;
constexpr unsigned int VertexUV = 8;
constexpr unsigned int VertexWeights = 16;

constexpr size_t VertexPositionSize = 3;
constexpr size_t VertexNormalSize = 3;
constexpr size_t VertexColorSize = 3;
constexpr size_t VertexUVSize = 2;
constexpr size_t VertexWeightsSize = 8;

#pragma pack(push, 1)
class VertexFormat
{
public:
	VertexFormat() : VertexFormat(VertexPosition) {}
	VertexFormat(unsigned int attribs);

	inline bool operator == (const VertexFormat& other) const {
		return attribute == other.attribute;
	}

	inline bool Has(unsigned attrib) const {
		return (attribute & attrib) != 0;
	}

	inline size_t Stride() const
	{
		return stride;
	}

	inline size_t Offset(unsigned attrib) {
		return this->*offsets[attrib];
	}

private:
	typedef char VertexFormat::* member_pointer;
	static const member_pointer offsets[VertexWeights + 1];
private:
	unsigned short	attribute;
	unsigned char	stride;
	char			offsetPosition;
	char			offsetNormal;
	char			offsetColor;
	char			offsetUV;
	char			offsetWeights;
};
#pragma pack(pop)
