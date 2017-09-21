#include <SDL.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <iostream>
#include "Window.h"
#include "Game.h"


int main(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cerr << "SDL could not initialize!" << SDL_GetError() << "\n";
		return -1;
	}
	
	if (TTF_Init() < 0)
	{
		std::cerr << "TTF could not initialize!" << TTF_GetError() << "\n";
		return -1;
	}

	//Ensure that we close SDL and SDL TTF if the game crashes/is aborted
	atexit(SDL_Quit);
	atexit(TTF_Quit);

	GameOfLife myGameOfLife = GameOfLife();
	myGameOfLife.SetGameState(MENU);
	myGameOfLife.RandomizeBoard(10.0);

	bool gameRunning = true;

	while (gameRunning == true)
	{
		if (myGameOfLife.Play() > 0)
		{



			myGameOfLife.GetIsRunning(&gameRunning); //Check if we are still running the game
		}
		else
			gameRunning = false;
	}


	return 1;
}