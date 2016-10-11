#pragma once

struct Mesh;
//struct 
struct RenderTask
{
	const Mesh*			mesh;
	const float*		worldMatrix;
};
