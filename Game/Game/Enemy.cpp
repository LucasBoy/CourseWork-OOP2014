#include "Enemy.h"

Enemy::Enemy(const Coordinates& position, const Item& item) {
	this->MoveTo(position);
	this->item = item;

	attack += item.GetAttack();
	accuracy += item.GetAccuracy();
	defense += item.GetDefense();
}

Enemy::Enemy() {
	this->MoveTo(Coordinates(0, 0));
	this->item = Item();

	attack += item.GetAttack();
	accuracy += item.GetAccuracy();
	defense += item.GetDefense();
}

Enemy::Enemy(std::ifstream& file) : Creature(file), item(file) {
	
	attack += item.GetAttack();
	accuracy += item.GetAccuracy();
	defense += item.GetDefense();

	if (!file.good()) {
		throw "Enemy cannot be loaded!";
	}
}

Item Enemy::GetItem() const {
	return item;
}

void Enemy::Save(std::ofstream& file) const {
	Creature::Save(file);
	item.Save(file);

	if (!file.good()) {
		throw "Enemy not saved!";
	}
}