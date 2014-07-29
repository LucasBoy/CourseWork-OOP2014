#include <ostream>
#include "Item.h"
#include "Hero.h"
#include "Enemy.h"
#include "ItemType.h"

std::ostream& operator<<(std::ostream& os, const ItemType& it) {
	switch (it) {
		case ItemType::Shield:
			os << "Shield";
			break;
		case ItemType::Helmet:
			os << "Helmet";
			break;
		case ItemType::Sword:
			os << "Sword";
			break;
		case ItemType::Booster:
			os << "Booster";
			break;
	}

	return os;
}

std::ostream& operator<<(std::ostream& os, const Item& item) {
	os << item.GetType() << std::endl;

	if (item.GetDefense() != 0)
	    os << "\t" << "Defense bonus: " << item.GetDefense() << '\n';

	if (item.GetAttack() != 0)
		os << "\t" << "Attack bonus: " << item.GetAttack() << '\n';

	if (item.GetAccuracy() != 0)
		os << "\t" << "Accuracy bonus: " << item.GetAccuracy() << '\n';

	if (item.GetEnergy() != 0)
		os << "\t" << "Energy bonus: " << item.GetEnergy() << '\n';
    
	return os;
}

std::ostream& operator<<(std::ostream& os, const Hero& hero) {

	os << hero.GetName() << '\n'
		<< "--------------------\n"
		<< "\tHealth: " << hero.GetHealth() << '\n'
		<< "\tDefense: " << hero.GetDefense() << '\n'
		<< "\tAttack: " << hero.GetAttack() << '\n'
		<< "\tAccuracy: " << hero.GetAccuracy() << '\n'
		<< "\tEnergy: " << hero.GetEnergy() << "\n\n";
	return os;
}

std::ostream& operator<<(std::ostream& os, const Enemy& enemy) {
	os << "Enemy" << '\n'
		<< "--------------------\n"
		<< "\tHealth: " << enemy.GetHealth() << '\n'
		<< "\tDefense: " << enemy.GetDefense() << '\n'
		<< "\tAttack: " << enemy.GetAttack() << '\n'
		<< "\tAccuracy: " << enemy.GetAccuracy() << "\n\n";

	return os;
}