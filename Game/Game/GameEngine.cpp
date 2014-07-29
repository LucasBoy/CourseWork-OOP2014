#include "GameEngine.h"
#include "IOEngine.h"
#include "Constants.h"
#include <fstream>
#include <iostream>

using namespace Constants::Maps;
using namespace Constants::Input;

GameEngine::GameEngine() {
	// Count files in mapsPath (relative to root), starting with "Map"
	std::string command;
	command = "dir ";
	command += mapsPath;
	command += " /a /b | find \"";
	command += mapsPrefix;
	command += "\" /c > mapsCount.txt";
	system(command.c_str());
	std::ifstream f("mapsCount.txt");
	f >> mapsCount;
	f.close();
	remove("mapsCount.txt");

	sizeX = 0;
	sizeY = 0;
	currentMap = 1;

	InitializeGame();
}

GameEngine::~GameEngine() {
	DeleteData();
	delete hero;
	
}

void GameEngine::DeleteData() {
	for (size_t i = 0; i < sizeY; i++) {
		delete[] map[i];
	}

	delete[] map;

	for (size_t i = 0; i < enemies.size(); i++) {
		delete enemies[i];
	}

	enemies.clear();
}

GameEngine& GameEngine::GetInstance() {
	static GameEngine instance;
	return instance;
}

void GameEngine::InitializeMap(int mapNumber) {
	std::string fileName = "";

	fileName += mapsPath;
	fileName += "/";
	fileName += mapsPrefix;
	fileName += std::to_string(mapNumber);
	fileName += ".";
	fileName += mapsFileExtension;

	std::ifstream file(fileName);

	if (!file.good()) {
		IOEngine::Println("Cannot load map data!");

		if (IOEngine::YesNoQuestion("Do you want to save your game? (Y/N)")) 
			SaveGame();

		DeleteData();
		exit(1);
	}

	sizeX = 0;
	sizeY = 1; // Assuming there is no \n char on the last line

	// Number of '\n' chars is the number of lines
	while (file.get() != '\n') {
		++sizeX;
	}

	file.seekg(0, std::ios::beg);
	while (file) {
		if (file.get() == '\n') {
			++sizeY;
		}
	}

	if (sizeX <= 0 || sizeY <= 0) {
		IOEngine::Println("Invalid map data!");

		if (IOEngine::YesNoQuestion("Do you want to save your game? (Y/N)"))
			SaveGame();

		DeleteData();
		exit(1);
	}

	file.clear();
	file.seekg(0, std::ios::beg);
	try {
		map = new char*[sizeY];

		for (size_t i = 0; i < sizeY; i++) {
			map[i] = new char[sizeX + 2]; // \n and \0
			file.getline(map[i], sizeX + 1);
		}
	}
	catch (std::bad_alloc) {
		IOEngine::Println("Not enough memory! Exiting!");
		DeleteData();
		delete hero;
		file.close();
		exit(1);
	}
	file.close();
}

void GameEngine::RunGame() {

	InitializeMap(currentMap);
	InitializeFigures();

	char input = symbolEmpty; // Something to run the loop
	bool movement = false;

	while (input != buttonExit) {
		
		IOEngine::ClearScreen();
		IOEngine::DrawMap(map, sizeY);
		
		input = IOEngine::CharInput();

		movement = false;
		if (input == buttonUp || input == buttonDown || 
			input == buttonLeft || input == buttonRight) {

			movement = true;
		}

		if (movement) {
			Coordinates heroPosition = hero->GetPosition();
			Coordinates newHeroPosition;

			switch (input) {
				case buttonDown:
					newHeroPosition = Coordinates(heroPosition.GetX(), heroPosition.GetY() + 1);
					break;
				case buttonUp:
					newHeroPosition = Coordinates(heroPosition.GetX(), heroPosition.GetY() - 1);
					break;
				case buttonLeft:
					newHeroPosition = Coordinates(heroPosition.GetX() - 1, heroPosition.GetY());
					break;
				case buttonRight:
					newHeroPosition = Coordinates(heroPosition.GetX() + 1, heroPosition.GetY());
				default:
					break;
			}

			switch (GetSymbol(newHeroPosition)) { // Valid movement location?
				case symbolEmpty:
					MovementAction(heroPosition, newHeroPosition); // TODO : Portals?
					break;
				case symbolItem:
					ItemInteraction(newHeroPosition);
					MovementAction(heroPosition, newHeroPosition);
					break;
				case symbolEnemy:
					EnemyInteraction(newHeroPosition);
					break;
				default:
					break;
			}
		}
		else { // The input is not a movement action
			switch (input) {
				case buttonExit:
					if (IOEngine::YesNoQuestion("Do you want to save your game?"))
						SaveGame();
					//DeleteData();
					return;
				case buttonSave:
					SaveGame();
					break;
				case buttonStats:
					ShowStats();
					break;
			}
		}
	}
}

void GameEngine::InitializeFigures() {
	for (size_t y = 0; y < sizeY; y++) {
		for (size_t x = 0; x < sizeX; x++) {
			switch (GetSymbol(Coordinates(x, y))) {
				case symbolHero:
					hero->MoveTo(Coordinates(x, y));
					break;
				case symbolEnemy:
					enemies.push_back(new Enemy(Coordinates(x, y), Item()));
					break;
				case symbolItem:
					// Do nothing -> Items are created when absolutely needed. ItemInteraction()
					break;
			}
		}
	}

}

void GameEngine::MoveObject(const Coordinates& oldCs, const Coordinates& newCs) {
	SetSymbol(symbolEmpty, oldCs);
	SetSymbol(symbolHero, newCs);
}

void GameEngine::SetSymbol(char symbol, const Coordinates& pos) {
	if (pos.GetX() >= sizeX || pos.GetY() >= sizeY) 
		// throw "Out of range!";
		return;

	map[pos.GetY()][pos.GetX()] = symbol;
}

char GameEngine::GetSymbol(const Coordinates& coordinates) {
	size_t x = coordinates.GetX();
	size_t y = coordinates.GetY();

	if (x >= sizeX || y >= sizeY)
		return symbolEmpty;

	return map[y][x];
}

void GameEngine::MovementAction(const Coordinates& oldPos, const Coordinates& newPos) {
	if (newPos.GetX() <= 0 || newPos.GetX() >= sizeX - 1 ||
		newPos.GetY() <= 0 || newPos.GetY() >= sizeY - 1) { // Reached end of map? -> load new map

		DeleteData();

		// Heal with healthPerLevel
		hero->TakeHit(-Constants::InitialCreatureStats::healthPerLevel);

		if (++currentMap > mapsCount)
			currentMap = 1;

		InitializeMap(currentMap);
		InitializeFigures();
		return;
	}

	MoveObject(oldPos, newPos);
	hero->MoveTo(newPos);
}

void GameEngine::ItemInteraction(const Coordinates& position) {
	Item* item;
	try {
		item = new Item(currentMap);
	}
	catch (std::bad_alloc) {
		IOEngine::Println("Not enough memory! Try again!");
		return;
	}
	ItemType newItemType = item->GetType();
	IOEngine::ClearScreen();
	IOEngine::PrintHero(*hero);
	IOEngine::Println("Your new item:");
	IOEngine::PrintItem(*item);
	size_t invSize = 0;
	Item* heroInvContent;

	try { 
		heroInvContent = hero->GetBagContents(invSize);
	}
	catch (std::bad_alloc) {
		IOEngine::Println("Cannot load inventory!");
		return;
	}
	

	bool addToInventory = IOEngine::YesNoQuestion("Do you want to add this item to your inventory ? (Y / N)");

	if (addToInventory) {
		if (hero->HasSuchItem(newItemType) && newItemType != ItemType::Booster) {
			IOEngine::ClearScreen();
			IOEngine::Println("Your item:");
			for (size_t i = 0; i < invSize; i++) {
				if (heroInvContent[i].GetType() == newItemType) {
					IOEngine::PrintItem(heroInvContent[i]);
					break;
				}
			}
			IOEngine::Println("New item:");
			IOEngine::PrintItem(*item);
			addToInventory = IOEngine::YesNoQuestion("You already have similar item. Do you want to use the new one? (Y / N)");

			if (addToInventory) {
				hero->DropType(newItemType);
				hero->PickUp(item);
				SetSymbol(symbolEmpty, position);
				IOEngine::Println("You have a new item!");
			}
			else {
				delete item;
				IOEngine::Println("You decided not to pick up the item!");
				return;
			}
		}
		else {
			hero->PickUp(item);
			SetSymbol(symbolEmpty, position);
			IOEngine::Println("You have a new item!");
		}
	}
	else {
		IOEngine::Println("You decided not to pick up the item!");
		delete item;
	}

	IOEngine::Pause();

}

void GameEngine::ShowStats() {

	IOEngine::ClearScreen();
	IOEngine::PrintHero(*hero);
	size_t invSize = 0;
	Item* heroInvContent;

	try {
		heroInvContent = hero->GetBagContents(invSize);
	}
	catch (std::bad_alloc) {
		IOEngine::Println("Cannot load inventory!");
		return;
	}

	bool useBooster = false;
	if (invSize > 0) {
		for (size_t i = 0; i < invSize; i++) {
			std::cout << "(" << i + 1 << ") ";
			IOEngine::PrintItem(heroInvContent[i]);
		}
		if (hero->HasSuchItem(ItemType::Booster))
			useBooster = IOEngine::YesNoQuestion("Use a booster? (Y/N)");
	}
	else {
		IOEngine::Println("Inventory is empty!");
	}

	if (useBooster) {
		bool hasBoosters = false;
		for (size_t i = 0; i < invSize; i++) {
			if (heroInvContent[i].GetType() == Booster) {
				std::cout << "(" << i + 1 << ")";
				IOEngine::PrintItem(heroInvContent[i]);
				hasBoosters = true;
			}
		}
		if (hasBoosters) {
			size_t input = 0;
			std::cin.sync();
			while (input <= 0 || input > invSize) {
				IOEngine::Println("Booster number:");
				std::cin >> input;
				if (!std::cin.good()) {
					std::cin.clear();
					std::cin.sync();
					input = 0;
				}
			}

			if (hero->UseBooster(input - 1)) {
				IOEngine::Println("You used a booster!");
			}
			else {
				IOEngine::Println("Cannot find specified booster!");
			}
		}
		else {
			IOEngine::Println("You don't have boosters!");
		}
	}

	delete[] heroInvContent;
	IOEngine::Pause();

}

void GameEngine::InitializeHero(const char* name) {
	try {
		hero = new Hero(name);
	}
	catch (std::bad_alloc) {
		IOEngine::Println("Cannot initialize hero!");
		DeleteData();
		exit(1);
	}
}

void GameEngine::EnemyInteraction(const Coordinates& position) {
	
	Enemy* currentEnemy = NULL;
	size_t enemyIndex = 0;
	for (size_t i = 0; i < enemies.size(); i++) {
		if (enemies[i]->GetPosition() == position) {
			currentEnemy = enemies[i];
			enemyIndex = i;
			break;
		}
	}
	if (currentEnemy == NULL) {
		IOEngine::Println("Problem with enemy occured!");
		IOEngine::Pause();
		return;
	}

	IOEngine::ClearScreen();
	IOEngine::PrintEnemy(*currentEnemy);

	bool fight = IOEngine::YesNoQuestion("Do you want to fight this enemy? (Y / N)");

	if (fight) {
		SetSymbol(symbolEmpty, position);
		enemies.erase(enemies.begin() + enemyIndex);

		bool heroAlive = Battle(currentEnemy);

		if (heroAlive) {
			IOEngine::Println("You defeated your enemy! You deserve your reward!");
			IOEngine::Pause();
			ItemInteraction(position);
		}
		else {
			IOEngine::ClearScreen();
			IOEngine::Println("You died! Game over!");
			exit(0);
		}

	}
	else {
		IOEngine::Println("You ran away!");
	}

}

bool GameEngine::Battle(Enemy* enemy) {
	int heroDamage = 0;
	int enemyDamage = 0;
	int heroDefense = 0;
	int enemyDefense = 0;
	
	bool raiseShield = false;
	bool swingSword = false;
	bool heroHasSword = hero->HasSuchItem(ItemType::Sword);
	bool heroHasShield = hero->HasSuchItem(ItemType::Shield);

	while (enemy->GetHealth() > 0 && hero->GetHealth() > 0) {
		// Print stats
		IOEngine::ClearScreen();
		IOEngine::PrintHero(*hero);
		IOEngine::PrintEnemy(*enemy);

		// Load data
		heroDamage = hero->GetAttack();
		enemyDamage = enemy->GetAttack();
		heroDefense = hero->GetDefense();
		enemyDefense = enemy->GetDefense();

		enemyDamage -= heroDefense;
		heroDamage -= enemyDefense;

		heroDamage *= hero->GetAccuracy();
		heroDamage = heroDamage / 10;

		enemyDamage *= enemy->GetAccuracy();
		enemyDamage = enemyDamage / 10;

		std::string message;

		// Ask for user's special interactions
		if (heroHasShield && hero->GetEnergy() >= 10) {
			message = "Do you want to raise your shield? (-";
			message += std::to_string(10);
			message += " energy / +";
			message += std::to_string(Constants::InitialItemStats::maxDefense / 2);
			message += " defense) (Y / N)";

			raiseShield = IOEngine::YesNoQuestion(message.c_str());

			if (raiseShield) {
				heroDefense += Constants::InitialItemStats::maxDefense / 2;
				hero->UseEnergy(10);
				IOEngine::Println("Shield raised!");
			}
			else {
				IOEngine::Println("Shield is down!");
			}
		}

		if (heroHasSword && hero->GetEnergy() >= 20) {
			message = "Do you want to swing your sword? (-";
			message += std::to_string(20);
			message += " energy / +";
			message += std::to_string(Constants::InitialItemStats::maxAttack / 2);
			message += "attack) (Y / N)";

			swingSword = IOEngine::YesNoQuestion(message.c_str());

			if (swingSword) {
				heroDamage += Constants::InitialItemStats::maxAttack / 2;
				hero->UseEnergy(20);
				IOEngine::Println("You swing your sword towards the enemy!");
			}
			else {
				IOEngine::Println("Your sword will rest now!");
			}
		}

		// Prevent false healing 
		if (enemyDamage < 0) 
			enemyDamage = 2;
		if (heroDamage < 0)
			heroDamage = 5;

		// Apply damages
		hero->TakeHit(enemyDamage);
		enemy->TakeHit(heroDamage);
		IOEngine::Pause();
	}

	if (hero->GetHealth() > 0) {
		return true;
	}

	return false;
}

void GameEngine::InitializeGame() {
	IOEngine::Println("Do you want to start a new game or load a save file?");
	IOEngine::Println("new -> Start a new game");
	IOEngine::Println("load -> Load a previously saved game");

	char* choice;
	do {
		choice = IOEngine::StringInput();
	} while (strcmp(choice, "load") != 0 && strcmp(choice, "new") != 0);

	if (strcmp(choice, "new") == 0) {

		IOEngine::Println("Input your name (at least 3 symbols):");
		char* name;
		do {
			name = IOEngine::StringInput();
		} while (strlen(name) < 3);

		InitializeHero(name);

		delete name;
	}
	else {
		LoadGame();
	}

	delete choice;

}

void GameEngine::SaveGame() const{
	IOEngine::ClearScreen();
	IOEngine::Println("Input the name of the save file:");
	char* fileName = IOEngine::StringInput();
	
	std::ofstream file(fileName, std::ios::binary);
	try {

		hero->Save(file);

		//size_t enemiesCount = enemies.size();

		//file.write((char*)&enemiesCount, sizeof(enemiesCount));

		//for each (Enemy* enemy in enemies) {
		//	enemy->Save(file);
		//}

		file.write((char*)&currentMap, sizeof(currentMap));
		IOEngine::Println("Game saved to file:");
		IOEngine::Println(fileName);
	}
	catch (const char* error) {
		IOEngine::Println("Game could not be saved:");
		IOEngine::Println(error);
	}

	if (!file.good()) {
		IOEngine::Println("Game could not be saved:");
		IOEngine::Println("File error!");
	}

	delete fileName;
	IOEngine::Pause();
}

void GameEngine::LoadGame() {
	IOEngine::ClearScreen();
	IOEngine::Println("Input the name of the save file:");
	char* fileName = IOEngine::StringInput();

	std::ifstream file(fileName, std::ios::binary);
	try {
		hero = new Hero(file);

		//size_t enemiesCount;
		//file.read((char*)&enemiesCount, sizeof(enemiesCount));

		//for (size_t i = 0; i < enemiesCount; i++) {
		//	enemies.push_back(new Enemy(file));
		//}

		file.read((char*)&currentMap, sizeof(currentMap));

	}
	catch (const char* error) {
		IOEngine::Println(error);
		exit(1);
	}
	IOEngine::Println("Game loaded! Initializing...");
	IOEngine::Pause();
}
