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

void Entity::OnUpdate(float deltaTime)
{
}

void Entity::UpdateMatrix()
{
	// TODO
	std::vector<Entity*> stack;
	Entity* topmost = nullptr;
	Entity* p = this;

	while (p != nullptr)
	{
		if (p->modified)
		{
			topmost = p;
		}
		p = p->parent;
	}

	if (nullptr == topmost)
	{
		return;
	}

	p = this;
	while (p != topmost)
	{
		stack.push_back(p);
		p = p->parent;
	}
	stack.push_back(topmost);

	glm::mat4 worldMat(1.0f);
	if (nullptr != topmost->parent)
	{
		worldMat = *reinterpret_cast<glm::mat4*>(topmost->parent->worldMatrix);
	}

	for (size_t i = stack.size() - 1; i >= 0; --i)
	{
		glm::mat4& mat = *reinterpret_cast<glm::mat4*>(stack[i]->matrix);
		worldMat = worldMat * mat;
		*reinterpret_cast<glm::mat4*>(stack[i]->worldMatrix) = worldMat;
		stack[i]->dirty = false;
	}
}
