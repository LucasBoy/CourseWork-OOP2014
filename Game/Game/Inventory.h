#ifndef _INVENTORY
#define _INVENTORY
#include "Item.h"
#include <vector>
#include <fstream>

class Inventory {
public:
	Inventory() { }
	Inventory(std::ifstream& file);
	~Inventory();

	Item& operator[](size_t pos) const;
	void Add(Item* item);
	void Remove(size_t pos);
	void RemoveType(const ItemType& type);
	size_t GetSize() const { return items.size(); }
	bool Has(const ItemType& type) const;
	void Save(std::ofstream& file) const;

private:
	void operator=(const Inventory&);
	Inventory(const Inventory&);

	std::vector<Item*> items;

};

std::ostream& operator<<(std::ostream& os, const Inventory& bag);

#endif