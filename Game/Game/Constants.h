#ifndef _CONSTANTS
#define _CONSTANTS
/// Engine configurations
namespace Constants {
	namespace Maps {
		const char mapsPath[] = "Maps";
		const char mapsFileExtension[] = "txt";
		const char mapsPrefix[] = "Map";

		const char symbolHero = 'H';
		const char symbolWall = '#';
		const char symbolEmpty = ' ';
		const char symbolItem = 'I';
		const char symbolEnemy = 'E';
	}

	namespace Input {
		const char buttonUp = 'w';
		const char buttonDown = 's';
		const char buttonLeft = 'a';
		const char buttonRight = 'd';
		const char buttonExit = 'q';
		const char buttonYes = 'y';
		const char buttonNo = 'n';
		const char buttonStats = 'i';
		const char buttonSave = 'c';

		const bool ignoreMultipleActions = false;
	}

	namespace InitialCreatureStats {
		const int initialHealth = 100;
		const int initialAccuracy = 3;
		const int initialAttack = 30;
		const int initialDefense = 10;
		const int initialEnergy = 100;

		const int healthPerLevel = 20;
	}

	namespace InitialItemStats {
		const int maxAccuracy = 10;
		const int maxAttack = 9;
		const int maxDefense = 8;
		const int maxEnergy = 40;

		const int levelMultiplier = 2;
	}
}

#endif
