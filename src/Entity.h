#pragma once

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
	bool		dirty;				// if position information of this node changed within this frame
	bool		modified;			// if matrix have been updated during this frame
	float		matrix[16];
	float		worldMatrix[16];
};
