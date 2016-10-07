#pragma once

struct RenderTask;
class Engine;

class Entity
{
public:
	Entity();
	virtual ~Entity();

	inline Entity*		GetParent() { return parent; }

	virtual void		OnCreate();
	virtual void		OnUpdate(float deltaTime);
	virtual void		OnDestroy();

protected:
	friend class Engine;

	virtual RenderTask*	OnRender();

protected:
	Engine*		engine;
	Entity*		parent;
	float		matrix[16];
	float		worldMatrix[16];

private:
	void		UpdateMatrix();

private:
	bool		dirty;				// if position information of this node changed within this frame
	bool		modified;			// if matrix have been updated during this frame
};
