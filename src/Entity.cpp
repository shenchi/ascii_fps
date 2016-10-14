#include "Entity.h"

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

Entity::Entity()
	:
	engine(nullptr),
	parent(nullptr),
	visible(true),
	remove(false),
	dirty(false),
	modified(false),
	posX(0.0f), posY(0.0f), posZ(0.0f),
	rotX(0.0f), rotY(0.0f), rotZ(0.0f),
	scaleX(1.0f), scaleY(1.0f), scaleZ(1.0f)
{
	*reinterpret_cast<glm::mat4*>(matrix) = glm::mat4(1.0f);
	*reinterpret_cast<glm::mat4*>(worldMatrix) = glm::mat4(1.0f);
}

Entity::~Entity()
{
}

void Entity::OnCreate()
{
}

void Entity::OnUpdate(float deltaTime)
{
}

void Entity::OnOverlay()
{
}

void Entity::OnDestroy()
{
}

void Entity::SetPosition(float x, float y, float z)
{
	posX = x;
	posY = y;
	posZ = z;
	dirty = true;
}

void Entity::SetRotation(float x, float y, float z)
{
	rotX = x;
	rotY = y;
	rotZ = z;
	dirty = true;
}

void Entity::SetScale(float x, float y, float z)
{
	scaleX = x;
	scaleY = y;
	scaleZ = z;
	dirty = true;
}

void Entity::UpdateLocalMatrix()
{
	if (!dirty) return;
	*reinterpret_cast<glm::mat4*>(matrix) =
		glm::translate(glm::vec3(posX, posY, posZ)) *
		glm::eulerAngleYXZ(rotY, rotX, rotZ) *
		glm::scale(glm::vec3(scaleX, scaleY, scaleZ));
	dirty = false;
}

void Entity::UpdateWorldMatrix()
{
	Entity* topmost = nullptr;
	Entity* p = this;

	while (p != nullptr)
	{
		if (p->dirty)
		{
			topmost = p;
		}
		if (p->modified)
		{
			topmost = p;
			break;
		}
		p = p->parent;
	}

	if (nullptr == topmost)
	{
		return;
	}

	std::vector<Entity*> stack;

	p = this;
	while (p != topmost)
	{
		stack.push_back(p);
		p = p->parent;
	}
	stack.push_back(topmost);

	int i = int(stack.size()) - 1;
	while (i >= 0 && stack[i]->modified)
	{
		i--;
	}

	if (i < 0)
	{
		return;
	}

	glm::mat4 worldMat(1.0f);
	if (nullptr != stack[i]->parent)
	{
		worldMat = *reinterpret_cast<glm::mat4*>(stack[i]->parent->worldMatrix);
	}

	for (; i >= 0; --i)
	{
		stack[i]->UpdateLocalMatrix();
		glm::mat4& localMat = *reinterpret_cast<glm::mat4*>(stack[i]->matrix);
		worldMat *= localMat;
		*reinterpret_cast<glm::mat4*>(stack[i]->worldMatrix) = worldMat;
		stack[i]->dirty = false;
		stack[i]->modified = true;
	}
}

void Entity::UpdateRemoveFlag()
{
	if (remove)
		return;

	Entity* topmost = nullptr;
	Entity* p = this;

	while (p != nullptr)
	{
		if (p->remove)
		{
			topmost = p;
		}
		p = p->parent;
	}

	if (nullptr == topmost)
		return;

	p = this;
	while (p != topmost)
	{
		p->remove = true;
		p = p->parent;
	}
}

RenderTask * Entity::OnRender()
{
	return nullptr;
}
