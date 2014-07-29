#include "Inventory.h"

Inventory::Inventory(std::ifstream& file) {
	size_t size = 0;
	file.read((char*)&size, sizeof(size));

	if (size < 0 || !file.good()) {
		throw "Invalid save file!";
	}

	for (size_t i = 0; i < size; i++) {
		items.push_back(new Item(file));
	}
}

Inventory::~Inventory() {
	for each (Item* item in items) {
		delete item;
	}
}

Item& Inventory::operator[](size_t i) const {
	return *items[i];
}

void Inventory::Add(Item* item) {
	items.push_back(item);
}

void Inventory::RemoveType(const ItemType& type) {
	for (size_t i = 0; i < items.size(); i++) {
		if (items[i]->GetType() == type) {

			Remove(i);

			return;
		}
	}
}

void Inventory::Remove(size_t index) {
	if (index >= GetSize()) {
		// throw "Index out of range!";
		return;
	}

	delete items[index];
	items.erase(items.begin() + index);
}

bool Inventory::Has(const ItemType& type) const {
	for each (Item* item in items) {
		if (item->GetType() == type) {
			return true;
		}
	}
	return false;
}

void Inventory::Save(std::ofstream& file) const{
	size_t size = GetSize();

	file.write((char*)&size, sizeof(size));

	for each (Item* item in items) {
		item->Save(file);
	}

	if (!file.good()) {
		throw "Inventory not saved!";
	}

}
