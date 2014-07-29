#ifndef _CREATURE
#define _CREATURE
#include "Coordinates.h"
#include <fstream>

class Creature {
public:
	Creature();
	Creature(const Creature&);
	Creature(std::ifstream& file);
	Creature& operator=(const Creature&);
	virtual ~Creature();

	void MoveTo(const Coordinates&);
	Coordinates GetPosition() const;
	int GetHealth() const { return health; }
	int GetAttack() const { return attack; }
	int GetDefense() const { return defense; }
	int GetAccuracy() const { return accuracy; }
	void TakeHit(int damage) { this->health -= damage; }

	void Save(std::ofstream& file) const;

protected:
	int health;
	int attack;
	int accuracy;
	int defense;
	Coordinates* position;
};
#endif