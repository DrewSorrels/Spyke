#include <stack>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "stdafx.h"
#include "game.hpp"
#include "game_state.hpp"
#include "texture_manager.hpp"

void Game::loadTextures()
{
	texmgr.loadTexture("grass", "assets/grass.png");
	texmgr.loadTexture("dirt", "assets/dirt.png");
	texmgr.loadTexture("stone", "assets/stone.png");

	texmgr.loadTexture("background", "assets/background.png");
}

void Game::loadTiles()
{
	Animation staticAnim(0, 0, 1.0f); // For any tile that does not move
	tiles["grass"] =
		Tile(this->tileSize, 1, texmgr.getRef("grass"), { staticAnim }, TileType::GRASS);
	tiles["dirt"] =
		Tile(this->tileSize, 1, texmgr.getRef("dirt"), { staticAnim }, TileType::DIRT);
	tiles["stone"] =
		Tile(this->tileSize, 1, texmgr.getRef("stone"), { staticAnim }, TileType::STONE);

	return;
}

void Game::pushState(GameState* state)
{
	this->states.push(state);
	return;
}

void Game::popState()
{
	delete this->states.top();
	this->states.pop();

	return;
}

void Game::changeState(GameState* state)
{
	if (!this->states.empty())
		popState();

	pushState(state);
	return;
}

GameState* Game::peekState()
{
	if (this->states.empty()) return nullptr;
	return this->states.top();
}

void Game::gameLoop()
{
	sf::Clock clock;

	while (this->window.isOpen())
	{
		sf::Time elapsed = clock.restart();
		float dt = elapsed.asSeconds();

		if (peekState() == nullptr) continue;
		peekState()->handleInput();
		peekState()->update(dt);

		this->window.clear(sf::Color::Black);
		peekState()->draw(dt);
		this->window.display();
	}
}

Game::Game()
{
	this->loadTextures();
	this->loadTiles();

	this->window.create(sf::VideoMode(800, 600), "Spyke");
	this->window.setFramerateLimit(60);

	this->background.setTexture(this->texmgr.getRef("background"));
}

Game::~Game()
{
	// Clear all states from the stack to remove references
	while (!this->states.empty()) popState();
}