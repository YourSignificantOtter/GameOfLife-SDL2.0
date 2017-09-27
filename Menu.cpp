#include "Menu.h"

Menu::Menu(int width, int height)
{
	windowWidth = width;
	windowHeight = height;
	CreateButtons();
}

Menu::~Menu()
{

}

void Menu::CreateButtons()
{
	buttons[OPEN_MENU_BTN] = { windowWidth - 50, windowHeight - 50, 40, 40 };
	buttons[SET_GRID_SIZE_BTN] = { windowWidth / 2 - 20, windowHeight / 2 - 100, 40, 40 };
	buttons[SET_SCREEN_SIZE_BTN] = { windowWidth / 2 - 20, windowHeight / 2, 40, 40 };
	buttons[RANDOMIZE_BTN] = { windowWidth / 2 - 20, windowHeight / 2 + 100, 40, 40 };
}

int Menu::CheckButtonPress(int mouseX, int mouseY)
{
	for (int i = 0; i < NUM_BUTTON_TYPE; i++)
	{
		if (mouseX >= buttons[i].x && mouseX <= buttons[i].x + buttons[i].w)
		{
			if (mouseY >= buttons[i].y && mouseY <= buttons[i].y + buttons[i].h)
			{
				return i;
			}
		}
	}
	return -1; //returns negative if no buttons were pressed
}

int Menu::WindowResized(int newWidth, int newHeight)
{
	windowWidth = newWidth;
	windowHeight = newHeight;
	CreateButtons();
	return 1;
}

SDL_Rect *Menu::GetButtons()
{
	return buttons;
}

int Menu::GetNumButtons()
{
	return NUM_BUTTON_TYPE;
}
