#ifndef GAME_HPP
#define GAME_HPP

#include <stack>
#include <SFML/Graphics.hpp>

class GameState;

class Game
{
public:
	static void start();

	std::stack<GameState*> states;

	void pushState(GameState* state);
	void popState();
	void changeState(GameState* state);
	GameState* peekState();

	void gameLoop();

	Game();
	~Game();

private:

	
	static sf::RenderWindow _window;
};

#endif /* GAME_HPP */