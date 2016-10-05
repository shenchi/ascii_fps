#pragma once

class Entity
{
public:

	inline Entity* GetParent() { return parent; }

	virtual void OnUpdate() = 0;

private:
	Entity*		parent;
};