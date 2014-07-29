#ifndef _IO_ENGINE
#define _IO_ENGINE
#include "Hero.h"
#include "Enemy.h"

class IOEngine {

public:
	static void DrawMap(char** map, const size_t sizeY);
	static void ClearScreen();
	static void PrintHero(const Hero& hero);
	static void PrintItem(const Item& item);
	static void PrintEnemy(const Enemy& enemy);
	static void Println(const char* string); // Prints the <string> on the console
	static bool YesNoQuestion(const char* question);
	static char* StringInput();
	static char CharInput();
	static int NumberInput();
	static void Pause();

private:
	IOEngine();
	IOEngine(const IOEngine&);
	void operator=(const IOEngine&);
	~IOEngine();
};

#endif