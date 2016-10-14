#pragma once

struct Mesh;
struct Pose;
//struct 
struct RenderTask
{
	const Mesh*			mesh;
	const float*		worldMatrix;
	const Pose*			pose;
};
