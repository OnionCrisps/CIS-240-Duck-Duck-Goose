#include "Game.h"

Game::Game()
{

}

Game::~Game()
{}


void Game::buildMenu()
{
	push_toOptions({ "Start", "Rules", "Quit" });
	push_Functions({
		[this]() {createCircle(); play(); },
		[this]() {},
		[this]() {exit(0); }});
	IBuilder::buildMenu();
}

Game::Player Game::createPlayer(std::string name)
{
	Player player;
	player.playerID = circle.getSize(); //assigned before the creation
	player.isEliminated = false;

	player.name = name;

	return player;
}

