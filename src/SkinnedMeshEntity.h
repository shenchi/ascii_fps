#pragma once

#include "MeshEntity.h"

struct Animation;
struct Pose;

class SkinnedMeshEntity : public MeshEntity
{
public:
	SkinnedMeshEntity();
	virtual ~SkinnedMeshEntity();

	virtual void	LoadMeshFromFile(const char* filename);

	bool			SetAction(size_t action);
	bool			SetFrame(int frame);

	virtual void	OnUpdate(float delta_time);

protected:
	bool			AdvanceFrame();
	void			UpdatePose();

protected:
	Animation*	anim;
	Pose*		pose;
	int			currentAction;
	int			startFrame;
	int			endFrame;
	int			currentSection;
	int			sectionStart;
	int			sectionEnd;
	int			currentFrame;
	bool		loop;
};
