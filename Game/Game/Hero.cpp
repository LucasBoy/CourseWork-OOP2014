#include "Hero.h"
#include "Constants.h"
#include <iostream>
Hero::Hero() : Creature(){
	name = "";
	energy = 0;

}

Hero::Hero(const char* name){
	using Constants::InitialCreatureStats::initialEnergy;
	this->name = name;
	this->energy = initialEnergy;
}

Hero::Hero(std::ifstream& file) : Creature(file), bag(file) {
	size_t nameLen;
	file.read((char*)&energy, sizeof(energy));
	file.read((char*)&nameLen, sizeof(nameLen));
	char* name = new char[nameLen + 1];
	file.read(name, nameLen);
	name[nameLen] = '\0';
	this->name = name;
	delete name;

	if (!file.good()) {
		throw "Hero cannot be loaded!";
	}

}

Hero::~Hero(){
	
}

void Hero::PickUp(Item* item) {
	bag.Add(item);
}

void Hero::DropType(const ItemType& type) {
    bag.RemoveType(type);
}

bool Hero::HasSuchItem(const ItemType& type) {
	return bag.Has(type);
}

int Hero::GetEnergy() const {
	int bonus = 0;
	for (size_t i = 0; i < bag.GetSize(); i++) {
		if (bag[i].GetType() != Booster)
			bonus += bag[i].GetEnergy();
	}
	return energy + bonus;
}

int Hero::GetAttack() const {
	int bonus = 0;
	for (size_t i = 0; i < bag.GetSize(); i++) {
		if (bag[i].GetType() != Booster)
		    bonus += bag[i].GetAttack();
	}

	return attack + bonus;
}

int Hero::GetDefense() const {
	int bonus = 0;
	for (size_t i = 0; i < bag.GetSize(); i++) {
		if (bag[i].GetType() != Booster)
			bonus += bag[i].GetDefense();
	}

	return defense + bonus;
}

int Hero::GetAccuracy() const {
	int bonus = 0;
	for (size_t i = 0; i < bag.GetSize(); i++) {
		if (bag[i].GetType() != Booster)
			bonus += bag[i].GetAccuracy();
	}

	return accuracy + bonus;
}

Item* Hero::GetBagContents(size_t &size) const {
	size = bag.GetSize();
	Item* content = new Item[size];

	for (size_t i = 0; i < size; i++) {
		content[i] = bag[i];
	}

	return content;
}

bool Hero::UseBooster(size_t invIndex) {
	Item& booster = bag[invIndex];
	if (booster.GetType() == Booster) {
		accuracy += booster.GetAccuracy();
		attack += booster.GetAttack();
		defense += booster.GetDefense();
		energy += booster.GetEnergy();
		
		bag.Remove(invIndex);
		return true;
	}
	return false;
}

void Hero::UseEnergy(int ammount) {
	energy -= ammount;
}

void Hero::Save(std::ofstream& file) const {
	Creature::Save(file);
	bag.Save(file);
	
	size_t nameLen = name.length();
	char* nameToSave = new char[nameLen+1];
	strcpy_s(nameToSave, nameLen + 1, name.c_str());

	file.write((char*)&energy, sizeof(energy));
	file.write((char*)&nameLen, sizeof(nameLen));
	file.write((char*)nameToSave, nameLen);

	delete nameToSave;

	if (!file.good()) {
		throw "Hero cannot be saved!";
	}

}