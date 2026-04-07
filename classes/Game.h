#pragma once
#include "../classes/IBuilder.h"
#include "../classes/DCList.h"
class Game {
public:
	Game();
	~Game();
	//system to build the menu.
	//system to get players.
	//system to make players.
	//system to assign players a role.
	//system to make the circle.
	//system to render the circle.
	//system to make the cooking pot.
	//system to render the cooking pot.
	/* Main Game Rules/Functionality
	 * choose the "it"
	 * have the "it" pick someone (duck duck duck goose)
	 * anticlockwise or clockwise choice
	 * The tapper must tap a key to run around the ring (quick time event)
	 * If they don't tap enough, then the "it" catches them and they get put in the cooking pot
	 * 
	*/
private:
	enum Role //storage structure for each type
	{
		TAPPER,
		GOOSE,
		IT
	};

	struct Player {
		Role status;
		int listIndex;
		bool isEliminated = false;
	};
};