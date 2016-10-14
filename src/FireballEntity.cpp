#include "FireballEntity.h"

#include "RenderTask.h"
#include "BuiltInShaders.h"

#include "MapManager.h"

#include <glm/glm.hpp>

using namespace glm;

FireballEntity::FireballEntity()
{
	task->vertexShader = BuiltInShaders::VertexShaderIndex::vsUnlit;
	speed = 20.0f;
}

void FireballEntity::OnCreate()
{
	LoadMeshFromFile("../assets/fireball.mesh");
}

void FireballEntity::OnUpdate(float deltaTime)
{
	vec3& dir = *reinterpret_cast<vec3*>(&dirX);
	vec3 pos = vec3(GetPositionX(), GetPositionY(), GetPositionZ());

	pos += dir * deltaTime * speed;
	
	if (MapManager::instance()->CollideWithMap(pos.x, pos.z, 0.1f) || pos.y <= 0.0f || pos.y >= 2.0f)
	{
		Destroy();
	}

	SetPosition(pos.x, pos.y, pos.z);
}

