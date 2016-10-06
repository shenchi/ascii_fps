#pragma once

class Engine;

class Entity
{
public:
	Entity();
	~Entity();

	inline Entity* GetParent() { return parent; }

	virtual void OnUpdate(float deltaTime);

private:
	friend class Engine;
	void		UpdateMatrix();

private:
	Entity*		parent;
	bool		dirty;
	bool		modified;
	float		matrix[16];
	float		worldMatrix[16];
};
