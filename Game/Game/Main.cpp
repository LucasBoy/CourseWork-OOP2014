#include <iostream>
#include <fstream>
#include "GameEngine.h"
#include "IOEngine.h"

int main() {

	GameEngine& engine(GameEngine::GetInstance());

	engine.RunGame();

	return 0;
}
