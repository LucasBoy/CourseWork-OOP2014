#include "Item.h"
#include "Constants.h"
#include <cstdlib>
#include <time.h>
#include <ostream>
using namespace Constants::InitialItemStats;

Item::Item() {
	RandomizeStats();
}

Item::Item(int levelBonus) {
	RandomizeStats();
	int bonus = levelBonus*levelMultiplier;
	attackBonus += bonus;
	defenseBonus += bonus;
	accuracyBonus += bonus;
	energyBonus += bonus;
}

Item::Item(int attack, int defense, int energy, int accuracy, ItemType type) {
	SetStats(attack, defense, energy, accuracy);
	this->type = type;
}

Item::Item(std::ifstream& file) {
	int attack = 0;
	int	defense = 0;
	int	energy = 0;
	int	accuracy = 0;
	ItemType type = ItemType::Booster; // 0

	file.read((char*)&attack, sizeof(attack));
	file.read((char*)&defense, sizeof(defense));
	file.read((char*)&energy, sizeof(energy));
	file.read((char*)&accuracy, sizeof(accuracy));
	file.read((char*)&type, sizeof(type));

	if (!file.good()) {
		throw "Item cannot be loaded!";
	}

	if (attack > maxAttack || defense > maxDefense ||
		energy > maxEnergy || accuracy > maxAccuracy) {
		throw "Invalid data!";
	}

	SetStats(attack, defense, energy, accuracy);
	this->type = type;

}

void Item::RandomizeStats() {

	srand((unsigned int)time(NULL)); // Seed from current time

	int attack = rand() % (maxAttack + 1);
	int defense = rand() % (maxDefense + 1);
	int energy = rand() % (maxEnergy + 1);
	int accuracy = rand() % (maxAccuracy + 1);

	SetStats(attack, defense, energy, accuracy);

	type = ItemType(rand() % 4); // 4 types of items

}

void Item::SetStats(int attack, int defense, int energy, int accuracy) {
	attackBonus = attack;
	defenseBonus = defense;
	accuracyBonus = accuracy;
	energyBonus = energy;
}

void Item::Save(std::ofstream& file) const {
	file.write((char*)&attackBonus, sizeof(attackBonus));
	file.write((char*)&defenseBonus, sizeof(defenseBonus));
	file.write((char*)&energyBonus, sizeof(energyBonus));
	file.write((char*)&accuracyBonus, sizeof(accuracyBonus));
	file.write((char*)&type, sizeof(type));

	if (!file.good()) {
		throw "Item not saved!";
	}

}
