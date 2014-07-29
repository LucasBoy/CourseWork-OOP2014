#ifndef _GAME_ENGINE
#define _GAME_ENGINE
#include "Coordinates.h"
#include "Hero.h"
#include "Enemy.h"
#include <vector>

class GameEngine {
public:
	void RunGame();
	static GameEngine& GetInstance();

private:
	GameEngine();
	~GameEngine();
	GameEngine(const GameEngine&);
	void operator=(const GameEngine&);

	void MoveObject(const Coordinates& currentPosition, const Coordinates& newPosition);
	bool CheckClearance(const Coordinates& position);
	char GetSymbol(const Coordinates& position);
	void SetSymbol(char symbol, const Coordinates& position);
	
	void ShowStats();
	void MovementAction(const Coordinates& oldPos, const Coordinates& newPos);
	void ItemInteraction(const Coordinates& position);
	void EnemyInteraction(const Coordinates& position);
	bool Battle(Enemy* enemy);

	void InitializeHero(const char* name);
	void InitializeMap(int number);
	void InitializeFigures();
	void InitializeGame();

	void DeleteData();

	void SaveGame() const;
	void LoadGame();

	char** map;
	size_t sizeX;
	size_t sizeY;
	Hero* hero;
	std::vector<Enemy*> enemies;
	int currentMap;
	int mapsCount;
	
};

#endif