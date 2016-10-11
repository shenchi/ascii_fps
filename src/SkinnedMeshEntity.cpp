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
	currentFrame(-1)
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

void SkinnedMeshEntity::SetAction(size_t action)
{
	if (action < anim->actions.size())
	{
		currentAction = int(action);
		startFrame = anim->actions[currentAction].startFrame;
		endFrame = anim->actions[currentAction].endFrame;

		currentFrame = startFrame;
	}
}

void SkinnedMeshEntity::SetFrame(int frame)
{
	if (currentAction < 0 || startFrame < 0 || endFrame < 0)
		return;

	if (frame < startFrame || frame > endFrame)
		return;

	Action& action = anim->actions[currentAction];

	// TODO find section
	for (size_t f = 0; f < action.keyFrames.size(); ++f)
	{
		
	}

}

void SkinnedMeshEntity::OnUpdate(float delta_time)
{
}
