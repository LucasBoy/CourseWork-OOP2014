#ifndef _ENEMY
#define _ENEMY
#include "Creature.h"
#include "Coordinates.h"
#include "Item.h"
#include <fstream>

class Enemy : public Creature {
public:
	Enemy();
	Enemy(const Coordinates&, const Item&);
	Enemy(std::ifstream& file);
	Item GetItem() const;

	void Save(std::ofstream& file) const;

private:
	Enemy(const Enemy&);
	Enemy& operator=(const Enemy&);

	Item item;

};

std::ostream& operator<<(std::ostream& os, const Enemy& e);
#endif
