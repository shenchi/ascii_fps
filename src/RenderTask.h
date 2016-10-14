#pragma once

struct Mesh;
struct Pose;
//struct 
struct RenderTask
{
	size_t				vertexShader;
	size_t				pixelShader;
	const Mesh*			mesh;
	const float*		worldMatrix;
	const Pose*			pose;
};
