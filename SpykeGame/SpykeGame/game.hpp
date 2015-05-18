#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <stack>
#include <map>
#include <string>

#include "tile.hpp"
#include "texture_manager.hpp"

class GameState;

class Game
{
private:

	void loadTextures();
	void loadTiles();

public:

	const static int tileSize = 8;

	sf::RenderWindow window;
	std::stack<GameState*> states;
	TextureManager texmgr;
	sf::Sprite background;

	std::map<std::string, Tile> tiles;

	void pushState(GameState* state);
	void popState();
	void changeState(GameState* state);
	GameState* peekState();

	void gameLoop();

	Game();
	~Game();
};

#endif /* GAME_HPP */