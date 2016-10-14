#pragma once

class Creature
{
public:
	inline void	Attack(Creature* c) { c->HP -= Damage; }

	inline void SetHP(int value) { HP = value; }
	inline int GetHP() const { return HP; }

	inline void SetDamage(int value) { Damage = value; }
	inline int GetDamage() const { return Damage; }

	inline void SetSpeed(float value) { Speed = value; }
	inline float GetSpeed() const { return Speed; }

	inline void SetRadius(float value) { Radius = value; }
	inline float GetRadius() const { return Radius; }

protected:
	int			HP;
	int			Damage;
	float		Speed;
	float		Radius;
};
