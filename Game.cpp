#include "Game.h"


GameOfLife::GameOfLife()
{
	isRunning = true;
	gameState = WAITING;

	gameWindow = SDL_CreateWindow("Game Of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (gameWindow == NULL)
	{
		gameError = "SDL Failed to create window!";
		exit(-1);
	}

	gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gameRenderer == NULL)
	{
		gameError = "SDL Failed to create renderer!";
		exit(-1);
	}

	Sans = TTF_OpenFont("OpenSans-Regular.ttf", 24);

	gameWindowSurface = SDL_GetWindowSurface(gameWindow);

	SDL_GetWindowSize(gameWindow, &gameWindowWidth, &gameWindowHeight);

	gridWidth = 100;
	gridHeight = 100;

	theGrid = new Cell*[gridWidth];
	for (int i = 0; i < gridWidth; i++)
	{
		theGrid[i] = new Cell[gridHeight];
	}

	fpsPrevTick = 0;
	fpsCurrent = 0;
	fpsFrames = 0; 
}

GameOfLife::~GameOfLife()
{
	isRunning = false;

	gameWindow = NULL;
	gameRenderer = NULL;
	fpsTexture = NULL;
	gameWindowSurface = NULL;
	fpsSurface = NULL;

	SDL_DestroyWindow(gameWindow);
	SDL_DestroyRenderer(gameRenderer);
	SDL_DestroyTexture(fpsTexture);
	SDL_FreeSurface(gameWindowSurface);
	SDL_FreeSurface(fpsSurface);

	for (int i = 0; i < gridWidth; i++)
		delete[] theGrid[i];
	delete[] theGrid;

	TTF_CloseFont(Sans);
}

int GameOfLife::Play()
{
	SDL_SetRenderDrawColor(gameRenderer, 255, 255, 255, 255);
	SDL_RenderClear(gameRenderer);

	//Check user inputs
	if (SDL_PollEvent(&gameEvent) != 0)
	{
		switch (gameEvent.type)
		{
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYDOWN:
			switch (gameEvent.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				isRunning = false;
				break;
			}
			break;
		default:
			//Do nothing
			break;
		}
	}

	TimeStep();
	RenderGrid();

	//I Assume this is where the painting is
	SDL_Color textColor = { 0, 0, 0};
	SDL_Color bgColor = { 255, 255, 255 };
	std::string fpsString = "FPS: " + std::to_string(fpsCurrent);
	fpsSurface = TTF_RenderText_Shaded(Sans, fpsString.c_str(), textColor, bgColor);
	fpsTexture = SDL_CreateTextureFromSurface(gameRenderer, fpsSurface);
	SDL_Rect fpsRect = { gameWindowWidth - 80, 0, 75, 60 };
	SDL_RenderCopy(gameRenderer, fpsTexture, NULL, &fpsRect);


	//Render current scene and count the FPS
	SDL_RenderPresent(gameRenderer);

	fpsFrames++;
	if (fpsPrevTick <= SDL_GetTicks() - FPS_INTERVAL * (float)1000)
	{
		fpsPrevTick = SDL_GetTicks();
		fpsCurrent = fpsFrames;
		fpsFrames = 0;
	}
	
	return 1;
}

int GameOfLife::RandomizeBoard(float percentAlive)
{
	srand(time(NULL));
	for(int i = 0; i < gridWidth; i++)
		for (int j = 0; j < gridHeight; j++)
		{
			int randomNum = rand() % 101; //number between 1 and 100
			if (randomNum >= percentAlive)
				theGrid[i][j].SetNextState(CELL_LIVING);
			else
				theGrid[i][j].SetNextState(CELL_DEAD);
		}

	return 1;
}

int GameOfLife::TimeStep()
{
	for (int i = 0; i < gridWidth; i++)
		for (int j = 0; j < gridHeight; j++)
		{
			theGrid[i][j].TimeStep();
		}
	return 1;
}

int GameOfLife::RenderGrid()
{

	return 1;
}

int GameOfLife::GetGridWidth(int *width)
{
	*width = gridWidth;
	return 1;
}

int GameOfLife::SetGridWidth(int width)
{
	gridWidth = width;
	return 1;
}

int GameOfLife::GetGridHeight(int *height)
{
	*height = gridHeight;
	return 1;
}

int GameOfLife::SetGridHeight(int height)
{
	gridHeight = height;
	return 1;
}

int GameOfLife::GetGameState(game_state_types *state)
{
	*state = gameState;
	return 1;
}

int GameOfLife::SetGameState(game_state_types state)
{
	gameState = state;
	return 1;
}

int GameOfLife::GetIsRunning(bool *running)
{
	*running = isRunning;
	return 1;
}

std::string GameOfLife::GetGameError()
{
	return gameError;
}