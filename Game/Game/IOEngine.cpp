#include "IOEngine.h"
#include "Hero.h"
#include <iostream>
#include "Constants.h"
using namespace Constants::Input;

void IOEngine::DrawMap(char** map, size_t rows) {
	for (size_t y = 0; y < rows; y++) {
		std::cout << map[y] << std::endl;
	}
}

void IOEngine::ClearScreen() {
	system("cls");
}

void IOEngine::PrintHero(const Hero& hero) {
	std::cout << hero;
}

void IOEngine::PrintEnemy(const Enemy& enemy) {
	std::cout << enemy;
}

void IOEngine::PrintItem(const Item& item) {
	std::cout << item;
}

void IOEngine::Println(const char* string) {
	std::cout << string << "\n";
}

bool IOEngine::YesNoQuestion(const char* question) {
	char input = buttonExit; // Something different from yes or no
	std::cin.sync();

	while (input != buttonYes && input != buttonNo) {
		Println(question);
		std::cin.get(input);
		std::cin.sync();
		input = tolower(input);
	}

	return input == buttonYes;
}

void IOEngine::Pause() {
	std::cin.sync();
	std::cin.get();
}

char* IOEngine::StringInput() {
	char* input = new char[101];
	std::cin.clear();
	std::cin.sync();
	std::cin.getline(input, 100);

	return input;
}

int IOEngine::NumberInput() {
	int number;
	do {
		std::cin.clear();
		std::cin.sync();
		std::cin >> number;
	} while (!std::cin.good());

	return number;
}

char IOEngine::CharInput() {
	char input;
	std::cin.get(input);

	if (ignoreMultipleActions) {
		std::cin.clear();
		std::cin.sync();
	}

	return tolower(input);
}