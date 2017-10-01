#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <string>

typedef enum
{
	OPEN_MENU_BTN,
	SET_GRID_SIZE_BTN,
	SET_SCREEN_SIZE_BTN,
	RANDOMIZE_BTN,
	NUM_BUTTON_TYPE
} button_types;

typedef enum
{
	GRID_SIZE_X_TXT,
	GRID_SIZE_Y_TXT,
	SCREEN_SIZE_X_TXT,
	SCREEN_SIZE_Y_TXT,
	RANDOMIZE_PERCENT_TXT,
	NUM_TXT_TYPE
} txtbox_types;

class Menu
{
public:
	Menu(int width, int height, int gridx, int gridy, TTF_Font *font);
	~Menu();

	int CheckMenuPress(int mouseX, int mouseY);
	int HandleTextInput(SDL_Event event, int focusIndex);
	SDL_Rect *GetButtons();
	int GetNumButtons();
	int GetNumTextBoxes();
	SDL_Rect *GetTextBoxes();
	std::string GetTextBoxValue(int);
	int WindowResized(int newWidth, int newHeight);
	int GridResized(int newWidth, int newHeight);
	int RenderMenu(SDL_Renderer *renderer);

private:
	void CreateButtons();
	void CreateTextBoxes();

	std::string textBoxValues[NUM_TXT_TYPE];

	SDL_Rect buttons[NUM_BUTTON_TYPE];
	SDL_Rect textBoxes[NUM_TXT_TYPE];
	TTF_Font *menuFont;

	int windowWidth;
	int windowHeight;
	int gridWidth;
	int gridHeight;
};