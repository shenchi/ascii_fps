#pragma once

struct HitInfo;

class Creature
{
public:
	inline void	Attack(Creature* c) { 
		c->HP -= Damage; 
		if (c->HP > 0)
		{
			c->OnDamage(Damage);
		}
		else
		{
			c->OnDie();
		}
	}

	virtual void OnDamage(int Damage) = 0;
	virtual void OnDie() = 0;
	virtual void OnHit(HitInfo* hitInfo) = 0;

	inline void SetMaxHP(int value) { MaxHP = value; SetHP(value); }
	inline int GetMaxHP() const { return MaxHP; }

	inline void SetHP(int value) { HP = value; }
	inline int GetHP() const { return HP; }

	inline void SetDamage(int value) { Damage = value; }
	inline int GetDamage() const { return Damage; }

	inline void SetSpeed(float value) { Speed = value; }
	inline float GetSpeed() const { return Speed; }

	inline void SetRadius(float value) { Radius = value; }
	inline float GetRadius() const { return Radius; }

protected:
	int			MaxHP;
	int			HP;
	int			Damage;
	float		Speed;
	float		Radius;
};
