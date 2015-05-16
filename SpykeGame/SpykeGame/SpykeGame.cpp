// SpykeGame.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "game.hpp"
#include "game_state_start.hpp"

int _tmain(int argc, _TCHAR* argv[])
{
	Game game;

	game.pushState(new GameStateStart(&game));
	game.gameLoop();

	return 0;
}

