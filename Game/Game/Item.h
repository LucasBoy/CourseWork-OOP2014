#include "ItemType.h"
#ifndef _ITEM
#define _ITEM
#include <fstream>
class Item {
public:
	Item();
	Item(int, int, int, int, ItemType);
	Item(int levelBonus);
	Item(std::ifstream& file);

	void Save(std::ofstream& file) const;

	int GetDefense() const { return defenseBonus; }
	int GetEnergy() const { return energyBonus; }
	int GetAccuracy() const { return accuracyBonus; }
	int GetAttack() const { return attackBonus; }
	ItemType GetType() const { return type; }

private:
	int defenseBonus;
	int attackBonus;
	int energyBonus;
	int accuracyBonus;
	ItemType type;

	void RandomizeStats();
	void SetStats(int, int, int, int);

};

std::ostream& operator<<(std::ostream& os, const Item& item);

#endif
