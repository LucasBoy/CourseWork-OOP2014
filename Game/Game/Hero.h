#ifndef _HERO
#define _HERO
#include "Creature.h"
#include "Inventory.h"
#include <string>
#include <fstream>

class Hero : public Creature {
public:
	Hero();
	Hero(const char*);
	Hero(std::ifstream& file);
	virtual ~Hero();

	const char* GetName() const { return name.c_str(); }
	int GetEnergy() const;
	int GetAttack() const;
	int GetDefense() const;
	int GetAccuracy() const;
	void UseEnergy(int ammount);

	bool HasSuchItem(const ItemType&);
	void PickUp(Item*);
	void DropType(const ItemType&);
	Item* GetBagContents(size_t &size) const;
	bool UseBooster(size_t invIndex);
	void Save(std::ofstream& file) const;

private:
	Hero(const Hero&);
	Hero& operator=(const Hero&);

	std::string name;
	int energy;
	Inventory bag;
	
};
std::ostream& operator<<(std::ostream& os, const Hero& h);

#endif