#include "SkinnedMeshEntity.h"

#include "Engine.h"
#include "RenderTask.h"

#include "Animation.h"
#include "BuiltInShaders.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

SkinnedMeshEntity::SkinnedMeshEntity()
	:
	anim(nullptr),
	pose(new Pose()),
	currentAction(-1),
	startFrame(-1),
	endFrame(-1),
	currentSection(-1),
	sectionStart(-1),
	sectionEnd(-1),
	currentFrame(-1),
	timePerFrame(1.0f / 24),
	loop(false),
	playing(false),
	timeElapsed(0.0f)
{
	task->vertexShader = BuiltInShaders::VertexShaderIndex::vsSkinnedMesh;
	task->pose = pose;
}

SkinnedMeshEntity::~SkinnedMeshEntity()
{
	delete pose;
}

void SkinnedMeshEntity::LoadMeshFromFile(const char * filename)
{
	MeshEntity::LoadMeshFromFile(filename);

	if (nullptr == mesh)
		return;

	anim = Engine::instance()->LoadAnimation(filename);

	if (nullptr == anim)
		return;

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
		startFrame = anim->actions[action].startFrame;
		endFrame = anim->actions[action].endFrame;

		SetFrame(startFrame);
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

	Action& action = anim->actions[static_cast<size_t>(currentAction)];
	size_t f = 0;

	for (; f + 1 < action.keyFrames.size(); ++f)
	{
		if (frame >= static_cast<int>(action.keyFrames[f].frame) && frame <= static_cast<int>(action.keyFrames[f + 1].frame))
		{
			currentSection = static_cast<int>(f);
			sectionStart = action.keyFrames[f].frame;
			sectionEnd = action.keyFrames[f + 1].frame;
			currentFrame = frame;

			UpdatePose();
			return true;
		}
	}

	return false;
}

void SkinnedMeshEntity::SetFrameRate(int fps)
{
	if (fps <= 0)
		return;
	timePerFrame = 1.0f / fps;
}

void SkinnedMeshEntity::OnUpdate(float delta_time)
{
	if (playing)
	{
		timeElapsed += delta_time;
		if (timeElapsed > timePerFrame)
		{
			timeElapsed -= timePerFrame;
			AdvanceFrame();
		}
	}
}

bool SkinnedMeshEntity::AdvanceFrame()
{
	if (currentAction < 0 || startFrame < 0 || endFrame < 0 || currentSection < 0)
		return false;

	Action& action = anim->actions[static_cast<size_t>(currentAction)];
	int numKeyFrames = static_cast<int>(action.keyFrames.size());

	int newFrame = currentFrame + 1;
	
	if (newFrame > sectionEnd)
	{
		if (currentSection >= numKeyFrames - 2)
		{
			if (!loop)
			{
				playing = false;
				return false;
			}
			currentSection = -1;
			newFrame = startFrame;
		}

		currentSection++;
		sectionStart = action.keyFrames[static_cast<size_t>(currentSection)].frame;
		sectionEnd = action.keyFrames[static_cast<size_t>(currentSection) + 1].frame;
	}

	currentFrame = newFrame;
	UpdatePose();

	return true;
}

void SkinnedMeshEntity::UpdatePose()
{
	if (currentAction < 0 || startFrame < 0 || endFrame < 0 || currentSection < 0)
		return;

	ArrayBuffer<int>& bones = anim->bones;
	Action& action = anim->actions[static_cast<size_t>(currentAction)];

	float t = 0.0f;
	if (sectionStart != sectionEnd)
	{
		t = (float)(currentFrame - sectionStart) / (sectionEnd - sectionStart);
	}

	for (size_t i = 0; i < bones.size(); ++i)
	{
		size_t f = static_cast<size_t>(currentSection);
		glm::vec3& T1 = *reinterpret_cast<glm::vec3*>(&action.keyFrames[f].bones[i].x);
		glm::vec3& T2 = *reinterpret_cast<glm::vec3*>(&action.keyFrames[f + 1].bones[i].x);

		glm::quat& R1 = *reinterpret_cast<glm::quat*>(&action.keyFrames[f].bones[i].qx);
		glm::quat& R2 = *reinterpret_cast<glm::quat*>(&action.keyFrames[f + 1].bones[i].qx);

		glm::vec3& S1 = *reinterpret_cast<glm::vec3*>(&action.keyFrames[f].bones[i].sx);
		glm::vec3& S2 = *reinterpret_cast<glm::vec3*>(&action.keyFrames[f + 1].bones[i].sx);

		glm::vec3& T = *reinterpret_cast<glm::vec3*>(&pose->bones[i].x);
		glm::quat& R = *reinterpret_cast<glm::quat*>(&pose->bones[i].qx);
		glm::vec3& S = *reinterpret_cast<glm::vec3*>(&pose->bones[i].sx);

		T = T1 * (1 - t) + T2 * t;
		R = glm::slerp(R1, R2, t);
		S = S1 * (1 - t) + S2 * t;

		glm::mat4& mat = *reinterpret_cast<glm::mat4*>(&pose->matrices[i * 16]);
		mat = glm::translate(T) * glm::toMat4(R) * glm::scale(S);

		int parent = bones[i];
		if (parent >= 0)
		{
			glm::mat4& matP = *reinterpret_cast<glm::mat4*>(&pose->matrices[static_cast<size_t>(parent) * 16]);
			mat = matP * mat;
		}
	}
}
