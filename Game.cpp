#include "Game.h"


GameOfLife::GameOfLife()
{
	isRunning = true;
	gameState = RUNNING;

	gameWindow = SDL_CreateWindow("Game Of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
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

	SDL_SetRenderDrawBlendMode(gameRenderer, SDL_BLENDMODE_BLEND);

	Sans = TTF_OpenFont("OpenSans-Regular.ttf", 24);

	gameWindowSurface = SDL_GetWindowSurface(gameWindow);

	SDL_GetWindowSize(gameWindow, &gameWindowWidth, &gameWindowHeight);

	gridWidth = 128;
	gridHeight = 72;

	gameMenu = new Menu(gameWindowWidth, gameWindowHeight, gridWidth, gridHeight, Sans);

	CreateGrid();

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
	gameWindowSurface = NULL;
	
	SDL_DestroyWindow(gameWindow);
	SDL_DestroyRenderer(gameRenderer);
	SDL_FreeSurface(gameWindowSurface);

	DeleteGrid();

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
		case SDL_WINDOWEVENT:
			if (gameEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
			{
				gameWindowWidth = gameEvent.window.data1;
				gameWindowHeight = gameEvent.window.data2;
				gameMenu->WindowResized(gameWindowWidth, gameWindowHeight);
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			int mouseX, mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);
			if (gameMenu->CheckMenuPress(mouseX, mouseY) == OPEN_MENU_BTN)
				gameState == RUNNING ? gameState = MENU : gameState == PAUSED ? gameState = MENU : gameState = RUNNING;
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
			case SDLK_p:
				gameState == RUNNING ? gameState = PAUSED : gameState = RUNNING;
				break;
			case SDLK_m:
				gameState = MENU;
				break;
			}
			break;
		default:
			//Do nothing
			break;
		}
	}

	switch (gameState)
	{
	case RUNNING:
		GameStateRunning();
		break;

	case PAUSED:
		GameStatePaused();
		break;

	case MENU:
		GameStateMenu();
		break;

	default:
		gameError = "Unkonown game state!";
		SDL_Delay(1000);
		break;
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

int GameOfLife::GameStateRunning()
{
	SDL_Texture *fpsTexture = NULL;
	SDL_Surface *fpsSurface = NULL; //Why does memory leak when I do this as a part of the class, and free it in dtor?
	SDL_Rect fpsRect = { gameWindowWidth - 75, 0, 75, 60 };

	SDL_Color textColor = { 0, 0, 0 };
	std::string fpsString = "FPS: " + std::to_string(fpsCurrent);
	fpsSurface = TTF_RenderText_Solid(Sans, fpsString.c_str(), textColor);
	fpsTexture = SDL_CreateTextureFromSurface(gameRenderer, fpsSurface);
	

	for (int x = 0; x < gridWidth; x++)
		for (int y = 0; y < gridHeight; y++)
		{
			int numNeighbors = GridGetNeighbors(x, y);
			theGrid[x][y].SetNextState(GridApplyRules(theGrid[x][y].GetCurrentState(), numNeighbors));
		}

	TimeStep();
	RenderGrid();
	SDL_RenderCopy(gameRenderer, fpsTexture, NULL, &fpsRect);

	SDL_SetRenderDrawColor(gameRenderer, 0x00, 0x00, 0xFF, 0x3F);
	SDL_Rect *menuButtons = gameMenu->GetButtons();
	SDL_RenderFillRect(gameRenderer, &menuButtons[0]);

	SDL_DestroyTexture(fpsTexture);
	SDL_FreeSurface(fpsSurface); //same mem leak question

	SDL_RenderPresent(gameRenderer);
	updatePrevTick = SDL_GetTicks();

	
	fpsFrames++;
	if (fpsPrevTick <= SDL_GetTicks() - FPS_INTERVAL * (float)1000)
	{
		fpsPrevTick = SDL_GetTicks();
		fpsCurrent = fpsFrames;
		fpsFrames = 0;
	}
	
	return 1;
}

int GameOfLife::GameStatePaused()
{
	if (gameEvent.type == SDL_KEYDOWN)
	{
		if (gameEvent.key.keysym.sym == SDLK_SPACE)
		{
			for (int x = 0; x < gridWidth; x++)
				for (int y = 0; y < gridHeight; y++)
				{
					int numNeighbors = GridGetNeighbors(x, y);
					theGrid[x][y].SetNextState(GridApplyRules(theGrid[x][y].GetCurrentState(), numNeighbors));
				}

			TimeStep();
			RenderGrid();
			SDL_RenderPresent(gameRenderer);
		}
	}
	return 1;
}

int GameOfLife::GameStateMenu()
{
	static int textBoxFocus = -1;

	gameMenu->RenderMenu(gameRenderer);

	SDL_RenderPresent(gameRenderer);
	
	if (gameEvent.type == SDL_MOUSEBUTTONDOWN)
	{
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);
		switch (gameMenu->CheckMenuPress(mouseX, mouseY))
		{
		case -1:
			textBoxFocus = -1;
			break;
		case OPEN_MENU_BTN:
			gameState = RUNNING;
			break;
		case SET_GRID_SIZE_BTN:
			SetGridWidth(std::stoi(gameMenu->GetTextBoxValue(GRID_SIZE_X_TXT)));
			SetGridHeight(std::stoi(gameMenu->GetTextBoxValue(GRID_SIZE_Y_TXT)));
			gameMenu->GridResized(gridWidth, gridHeight);
			break;
		case SET_SCREEN_SIZE_BTN:
			SDL_SetWindowSize(gameWindow, std::stoi(gameMenu->GetTextBoxValue(SCREEN_SIZE_X_TXT)), std::stoi(gameMenu->GetTextBoxValue(SCREEN_SIZE_Y_TXT)));
			gameWindowWidth = 640;
			gameWindowHeight = 360;
			gameMenu->WindowResized(gameWindowWidth, gameWindowHeight);
			break;
		case RANDOMIZE_BTN:
			RandomizeBoard(std::stoi(gameMenu->GetTextBoxValue(RANDOMIZE_PERCENT_TXT)));
			break;
		case GRID_SIZE_X_TXT + NUM_BUTTON_TYPE:
			textBoxFocus = GRID_SIZE_X_TXT;
			break;
		case GRID_SIZE_Y_TXT + NUM_BUTTON_TYPE:
			textBoxFocus = GRID_SIZE_Y_TXT;
			break;
		case SCREEN_SIZE_X_TXT + NUM_BUTTON_TYPE:
			textBoxFocus = SCREEN_SIZE_X_TXT;
			break;
		case SCREEN_SIZE_Y_TXT + NUM_BUTTON_TYPE:
			textBoxFocus = SCREEN_SIZE_Y_TXT;
			break;
		case RANDOMIZE_PERCENT_TXT + NUM_BUTTON_TYPE:
			textBoxFocus = RANDOMIZE_PERCENT_TXT;
			break;
		default:
			break;
		}
	}
	
	gameMenu->HandleTextInput(gameEvent, textBoxFocus);
	
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

	cellWidth = round(cellWidth);
	cellHeight = round(cellHeight);

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
	DeleteGrid();
	gridWidth = width;
	CreateGrid();
	return 1;
}

int GameOfLife::GetGridHeight()
{
	return gridHeight;
}

int GameOfLife::SetGridHeight(int height)
{
	DeleteGrid();
	gridHeight = height;
	CreateGrid();
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

int GameOfLife::DeleteGrid()
{
	for (int i = 0; i < gridWidth; i++)
		delete[] theGrid[i];
	delete[] theGrid;

	return 1;
}

int GameOfLife::CreateGrid()
{
	theGrid = new Cell*[gridWidth];
	for (int i = 0; i < gridWidth; i++)
	{
		theGrid[i] = new Cell[gridHeight];
	}
	RandomizeBoard(50);
	return 1;
}