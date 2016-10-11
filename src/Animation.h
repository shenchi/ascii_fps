#pragma once
#include "ArrayBuffer.h"

#pragma pack(push, 4) // default could be 8, :(
struct Bone
{
	float x, y, z;			// translation
	float qw, qx, qy, qz;	// rotation
	float sx, sy, sz;		// scale
};
#pragma pack(pop)

struct KeyFrame
{
	unsigned int		frame;
	ArrayBuffer<Bone>	bones;
};

struct Action
{
	unsigned int			startFrame;
	unsigned int			endFrame;
	ArrayBuffer<KeyFrame>	keyFrames;
};

struct Animation
{
	ArrayBuffer<int>		bones;
	ArrayBuffer<Action>		actions;
};

struct Pose
{
	ArrayBuffer<Bone>		bones;
	ArrayBuffer<float>		matrices;
};