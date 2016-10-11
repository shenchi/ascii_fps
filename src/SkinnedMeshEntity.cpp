#include "SkinnedMeshEntity.h"

#include "Animation.h"
#include "MeshLoader.h"

SkinnedMeshEntity::SkinnedMeshEntity()
	:
	anim(new Animation()),
	pose(new Pose()),
	currentAction(-1),
	startFrame(-1),
	endFrame(-1),
	currentSection(-1),
	sectionStart(-1),
	sectionEnd(-1),
	currentFrame(-1),
	loop(false)
{
}

SkinnedMeshEntity::~SkinnedMeshEntity()
{
	delete anim;
	delete pose;
}

void SkinnedMeshEntity::LoadMeshFromFile(const char * filename)
{
	if (!MeshLoader::LoadFromFile(filename, mesh, anim))
	{
		mesh->Clear();
		return;
	}

	pose->bones.release();
	pose->matrices.release();

	if (anim->bones.size() > 0)
	{
		pose->bones = ArrayBuffer<Bone>(anim->bones.size());
		pose->matrices = ArrayBuffer<float>(anim->bones.size() * 16);
	}

	SetAction(0);
}

bool SkinnedMeshEntity::SetAction(size_t action)
{
	if (action < anim->actions.size())
	{
		currentAction = int(action);
		startFrame = anim->actions[currentAction].startFrame;
		endFrame = anim->actions[currentAction].endFrame;

		currentFrame = startFrame;
		return true;
	}
	return false;
}

bool SkinnedMeshEntity::SetFrame(int frame)
{
	if (currentAction < 0 || startFrame < 0 || endFrame < 0)
		return false;

	if (frame < startFrame || frame > endFrame)
		return false;

	if (frame == currentFrame)
		return true;

	Action& action = anim->actions[currentAction];
	size_t f = 0;

	for (; f < action.keyFrames.size() - 1; ++f)
	{
		if (frame >= action.keyFrames[f].frame && frame < action.keyFrames[f + 1].frame)
		{
			currentSection = f;
			sectionStart = action.keyFrames[f].frame;
			sectionEnd = action.keyFrames[f + 1].frame;
			currentFrame = frame;
			return true;
		}
	}

	if (frame == action.keyFrames[f].frame)
	{
		currentSection = f;
		sectionEnd = sectionStart = action.keyFrames[f].frame;
		currentFrame = frame;
		return true;
	}

	return false;
}

void SkinnedMeshEntity::OnUpdate(float delta_time)
{
}

bool SkinnedMeshEntity::AdvanceFrame()
{
	if (currentAction < 0 || startFrame < 0 || endFrame < 0)
		return false;

	Action& action = anim->actions[currentAction];
	size_t numKeyFrames = action.keyFrames.size();

	int newFrame = currentFrame + 1;
	
	if (newFrame >= sectionEnd)
	{
		if (currentSection >= numKeyFrames - 1)
		{
			if (!loop)
				return false;
			currentSection = -1;
			newFrame = startFrame;
		}

		currentSection++;
		sectionStart = action.keyFrames[currentSection].frame;
		if (currentSection == numKeyFrames - 1)
			sectionEnd = sectionStart;
		else
			sectionEnd = action.keyFrames[currentSection + 1].frame;
	}

	currentFrame = newFrame;
	return true;
}

void SkinnedMeshEntity::UpdatePose()
{
	// TODO
}
