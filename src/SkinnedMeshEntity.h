#pragma once

#include "MeshEntity.h"

struct Animation;
struct Pose;

class SkinnedMeshEntity : public MeshEntity
{
public:
	SkinnedMeshEntity();
	virtual ~SkinnedMeshEntity();

	virtual void LoadMeshFromFile(const char* filename);

	void SetAction(size_t action);
	void SetFrame(int frame);

	virtual void OnUpdate(float delta_time);

protected:
	Animation*	anim;
	Pose*		pose;
	int			currentAction;
	int			startFrame;
	int			endFrame;
	int			currentFrame;
};
