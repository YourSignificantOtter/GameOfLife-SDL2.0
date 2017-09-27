#pragma once
#include <SDL.h>
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
	Menu(int width, int height);
	~Menu();

	int CheckButtonPress(int mouseX, int mouseY);
	SDL_Rect *GetButtons();
	int GetNumButtons();
	int WindowResized(int newWidth, int newHeight);

private:
	void CreateButtons();

	SDL_Rect buttons[NUM_BUTTON_TYPE];
	SDL_Rect textBoxes[NUM_TXT_TYPE];

	int windowWidth;
	int windowHeight;
};