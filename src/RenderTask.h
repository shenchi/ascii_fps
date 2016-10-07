#pragma once

class Mesh;

struct RenderTask
{
	const Mesh*			mesh;
	const float*		worldMatrix;
};
