#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <time.h>

#include "Cell.h"

typedef enum 
{
	WAITING,
	RUNNING,
	PAUSED,
	MENU
} game_state_types;


class GameOfLife
{
	public:
		//Constructors & Destructors
		GameOfLife();
		~GameOfLife();

		//Do Things
		int Play();
		int RandomizeBoard(float percentAlive);
		int TimeStep(void);
		int RenderGrid(void);
		int GridGetNeighbors(int x, int y);
		cell_states GridApplyRules(int currState, int numNeighbors);

		//Get and set
		int GetGridWidth(void);
		int SetGridWidth(int width);
		int GetGridHeight(void);
		int SetGridHeight(int height);
		int GetGameState(game_state_types *state);
		int SetGameState(game_state_types state);
		int GetIsRunning(bool *running);
		std::string GetGameError(void);
		

	private:
		int gridWidth;
		int gridHeight;
		Cell **theGrid;
		SDL_Rect **cells;

		int SetCellPositionSize(void);

		bool isRunning;
		game_state_types gameState;

		SDL_Event gameEvent;
		SDL_Window *gameWindow;
		SDL_Renderer *gameRenderer;
		SDL_Surface *gameWindowSurface, *fpsSurface;
		SDL_Texture *fpsTexture;
		int gameWindowWidth, gameWindowHeight;
		int gameWindowXPos, gameWindowYPos;

		TTF_Font *Sans;
		

		std::string gameError;

		//For FPS
		const float FPS_INTERVAL = 1.00;
		const float UPDATE_INTERVAL = 0.25;
		uint32_t updatePrevTick;
		uint32_t fpsPrevTick;
		uint32_t fpsCurrent;
		uint32_t fpsFrames; //numbers of frames that have occured
};