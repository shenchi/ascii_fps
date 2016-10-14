#pragma once

class Creature
{
public:
	inline void	Attack(Creature* c) { c->HP -= Damage; }

protected:
	int			HP;
	int			Damage;
};
