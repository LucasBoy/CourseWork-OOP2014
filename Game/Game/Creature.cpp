#include "Creature.h"
#include "Constants.h"
using namespace Constants::InitialCreatureStats;

Creature::Creature() {
	this->position = new Coordinates();
	this->attack = initialAttack;
	this->defense = initialDefense;
	this->health = initialHealth;
	this->accuracy = initialAccuracy;
}

Creature& Creature::operator=(const Creature& other) {
	if (this != &other) {
		this->health = other.health;
		this->attack = other.attack;
		this->accuracy = other.accuracy;
		this->defense = other.defense;

		this->position->SetX(other.position->GetX());
		this->position->SetY(other.position->GetY());
	}

	return *this;
}

Creature::Creature(const Creature& other) {
	this->health = other.health;
	this->attack = other.attack;
	this->accuracy = other.accuracy;
	this->defense = other.defense;

	this->position->SetX(other.position->GetX());
	this->position->SetY(other.position->GetY());
}

Creature::Creature(std::ifstream& file) {
	file.read((char*)&health, sizeof(health));
	file.read((char*)&attack, sizeof(attack));
	file.read((char*)&accuracy, sizeof(accuracy));
	file.read((char*)&defense, sizeof(defense));

	//position = new Coordinates(file);
	position = new Coordinates();
}

Creature::~Creature() {
	delete this->position;
}

void Creature::MoveTo(const Coordinates& newCs) {
	delete this->position;
	this->position = new Coordinates(newCs);

}

Coordinates Creature::GetPosition() const {
	return *this->position;
}

void Creature::Save(std::ofstream& file) const {
	
	file.write((char*)&health, sizeof(health));
	file.write((char*)&attack, sizeof(attack));
	file.write((char*)&accuracy, sizeof(accuracy));
	file.write((char*)&defense, sizeof(defense));

	//position->Save(file);

	if (!file.good()) {
		throw "Creature not saved!";
	}

}