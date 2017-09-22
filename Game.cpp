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

	updatePrevTick = 0;
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
			case SDLK_r:
				RandomizeBoard(75.0);
				break;
			}
			break;
		default:
			//Do nothing
			break;
		}
	}

	//I Assume this is where the painting is
	SDL_Color textColor = { 0, 0, 0};
	SDL_Color bgColor = { 255, 255, 255 };
	std::string fpsString = "FPS: " + std::to_string(fpsCurrent);
	fpsSurface = TTF_RenderText_Solid(Sans, fpsString.c_str(), textColor);
	fpsTexture = SDL_CreateTextureFromSurface(gameRenderer, fpsSurface);
	SDL_Rect fpsRect = { gameWindowWidth - 80, 0, 75, 60 };
	


	//Render current scene and count the FPS
	//if (SDL_GetTicks() - updatePrevTick >= UPDATE_INTERVAL * (float)1000)
	//{
	for (int x = 0; x < gridWidth; x++)
		for (int y = 0; y < gridHeight; y++)
		{
			int numNeighbors = GridGetNeighbors(x, y);
			theGrid[x][y].SetNextState(GridApplyRules(theGrid[x][y].GetCurrentState(), numNeighbors));
		}
	TimeStep();
	RenderGrid();
	SDL_RenderCopy(gameRenderer, fpsTexture, NULL, &fpsRect);
	SDL_RenderPresent(gameRenderer);
	updatePrevTick = SDL_GetTicks();
	//}

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
			theGrid[i][j].SetNextState((cell_states)((rand() / (float)100) < percentAlive));
			theGrid[i][j].TimeStep();
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

	float cellWidth = gameWindowWidth / (float)gridWidth;
	float cellHeight = gameWindowHeight / (float)gridHeight;

	cellToRender.w = cellWidth;
	cellToRender.h = cellHeight;

	for (int x = 0; x < gridWidth; x++)
	{
		for (int y = 0; y < gridHeight; y++)
		{
			cellToRender.x = x * cellWidth;
			cellToRender.y = y * cellHeight;
			
			if(theGrid[x][y].GetCurrentState() == CELL_LIVING)
				SDL_SetRenderDrawColor(gameRenderer, 0x00, 0xFF, 0x00, 0xFF);
			else
				SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

			SDL_RenderFillRect(gameRenderer, &cellToRender);
		}
	}
	return 1;
}

cell_states GameOfLife::GridApplyRules(int currState, int numNeighbors)
{
	if (currState == CELL_LIVING)
		if (numNeighbors == 2 || numNeighbors == 3)
			return CELL_LIVING;
		else
			return CELL_DEAD;

	if (currState == CELL_DEAD)
		if (numNeighbors == 3)
			return CELL_LIVING;
		else
			return CELL_DEAD;
}

int GameOfLife::GridGetNeighbors(int x, int y)
{
	int numNeighbors = 0;
	int neighborX = x, neighborY = y;

	for (int i = - 1; i <= 1; i++)
	{
		for (int j = - 1; j <= 1; j++)
		{
			if (i == 0 && j == 0)
				continue;

			neighborX = x + i;
			neighborY = y + j;

			if (x + i < 0 || x + i >= gridWidth)
				i > 0 ? neighborX = 0 : neighborX = gridWidth - 1;
			if (y + j < 0 || y + j >= gridHeight)
				j > 0 ? neighborY = 0 : neighborY = gridHeight - 1;

			numNeighbors += theGrid[neighborX][neighborY].GetCurrentState();
		}
	}
	return numNeighbors;
}

int GameOfLife::GetGridWidth()
{
	return gridWidth;
}

int GameOfLife::SetGridWidth(int width)
{
	gridWidth = width;
	return 1;
}

int GameOfLife::GetGridHeight()
{
	return gridHeight;
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