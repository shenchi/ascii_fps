#include "Entity.h"

#include <vector>
#include <glm/glm.hpp>

Entity::Entity()
	:
	parent(nullptr)
{
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

void Entity::OnDestroy()
{
}

void Entity::UpdateMatrix()
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

	size_t i = stack.size() - 1;
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
		worldMat = *reinterpret_cast<glm::mat4*>(topmost->parent->worldMatrix);
	}

	for (; i >= 0; --i)
	{
		glm::mat4& localMat = *reinterpret_cast<glm::mat4*>(stack[i]->matrix);
		worldMat *= localMat;
		*reinterpret_cast<glm::mat4*>(stack[i]->worldMatrix) = worldMat;
		stack[i]->dirty = false;
		stack[i]->modified = true;
	}
}

RenderTask * Entity::OnRender()
{
	return nullptr;
}
