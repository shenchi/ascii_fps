#pragma once

struct RenderTask;
class Engine;
class EntityManager;

class Entity
{
public:
	Entity();
	virtual ~Entity();

	inline Entity*		GetParent() { return parent; }
	inline void			SetParent(Entity* entity) { parent = entity; dirty = true; }

	inline bool			Enabled() const { return enable; }
	inline bool			Enable(bool e) { enable = e; }

	inline bool			Visible() const { return visible; }
	inline void			SetVisible(bool v) { visible = v; }

	inline void			Destroy() { remove = true; enable = false; visible = false; }

	virtual void		OnCreate();
	virtual void		OnUpdate(float deltaTime);
	virtual void		OnOverlay();
	virtual void		OnDestroy();

	void				SetPosition(float x, float y, float z);
	void				SetRotation(float x, float y, float z);
	void				SetScale(float x, float y, float z);

	inline float		GetPositionX() const { return posX; }
	inline float		GetPositionY() const { return posY; }
	inline float		GetPositionZ() const { return posZ; }

	inline float		GetRotationX() const { return rotX; }
	inline float		GetRotationY() const { return rotY; }
	inline float		GetRotationZ() const { return rotZ; }

	inline float		GetScaleX() const { return scaleX; }
	inline float		GetScaleY() const { return scaleY; }
	inline float		GetScaleZ() const { return scaleZ; }

	inline void			SetPositionX(float v) { posX = v; dirty = true; }
	inline void			SetPositionY(float v) { posY = v; dirty = true; }
	inline void			SetPositionZ(float v) { posZ = v; dirty = true; }

	inline void			SetRotationX(float v) { rotX = v; dirty = true; }
	inline void			SetRotationY(float v) { rotY = v; dirty = true; }
	inline void			SetRotationZ(float v) { rotZ = v; dirty = true; }

	inline void			SetScaleX(float v) { scaleX = v; dirty = true; }
	inline void			SetScaleY(float v) { scaleY = v; dirty = true; }
	inline void			SetScaleZ(float v) { scaleZ = v; dirty = true; }

	inline const float*	GetMatrix() { return matrix; }
	inline const float*	GetWorldMatrix() { return worldMatrix; }

protected:
	friend class Engine;
	friend class EntityManager;

	virtual RenderTask*	OnRender();

	void		UpdateLocalMatrix();
	void		UpdateWorldMatrix();
	void		UpdateRemoveFlag();

protected:
	Engine*		engine;
	Entity*		parent;

private:
	bool		enable;
	bool		visible;
	bool		remove;					// this entity will be removed soon
	bool		dirty;					// if position information of this node changed within this frame
	bool		modified;				// if matrix have been updated during this frame
	float		posX, posY, posZ;		// local coordinate
	float		rotX, rotY, rotZ;		// local euler roation
	float		scaleX, scaleY, scaleZ;	// local scale
	float		matrix[16];				// local transform matrix
	float		worldMatrix[16];		// world transform matrix
};
