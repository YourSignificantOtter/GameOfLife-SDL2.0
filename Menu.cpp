#include "Menu.h"

Menu::Menu(int width, int height, int gridx, int gridy, TTF_Font *font)
{
	windowWidth = width;
	windowHeight = height;
	gridWidth = gridx;
	gridHeight = gridy;
	CreateButtons();
	CreateTextBoxes();
	menuFont = font;
}

Menu::~Menu()
{

}

int Menu::RenderMenu(SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0x7F);
	SDL_RenderFillRects(renderer, buttons, NUM_BUTTON_TYPE);
	SDL_RenderFillRects(renderer, textBoxes, NUM_TXT_TYPE);

	SDL_Rect textRect;
	SDL_Color textColor = { 0, 0, 0 };

	for (int i = GRID_SIZE_X_TXT; i < NUM_TXT_TYPE; i++)
	{
		SDL_Surface *textSurface = NULL;
		SDL_Texture *textTexture = NULL;

		textSurface = TTF_RenderText_Solid(menuFont, textBoxValues[i].c_str(), textColor);
		textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		SDL_RenderCopy(renderer, textTexture, NULL, &textBoxes[i]);

		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(textTexture); //Memory leak if texture and surface ctor and dtor done outside of loop, why????
	}
	
	return 1;
}

void Menu::CreateButtons()
{
	buttons[OPEN_MENU_BTN] = { windowWidth - 50, windowHeight - 50, 40, 40 };
	buttons[SET_GRID_SIZE_BTN] = { windowWidth / 2 - 20, windowHeight / 2 - 100, 40, 40 };
	buttons[SET_SCREEN_SIZE_BTN] = { windowWidth / 2 - 20, windowHeight / 2, 40, 40 };
	buttons[RANDOMIZE_BTN] = { windowWidth / 2 - 20, windowHeight / 2 + 100, 40, 40 };
}

void Menu::CreateTextBoxes()
{
	textBoxes[GRID_SIZE_X_TXT] = { windowWidth / 4, windowHeight / 2 - 100, 100, 40 };
	textBoxes[GRID_SIZE_Y_TXT] = { windowWidth / 4 + 140, windowHeight / 2 - 100, 100, 40 };
	textBoxes[SCREEN_SIZE_X_TXT] = { windowWidth / 4, windowHeight / 2, 100, 40 };
	textBoxes[SCREEN_SIZE_Y_TXT] = { windowWidth / 4 + 140, windowHeight / 2, 100, 40 };
	textBoxes[RANDOMIZE_PERCENT_TXT] = { windowWidth / 4 + 140, windowHeight / 2 + 100, 100, 40 };

	textBoxValues[GRID_SIZE_X_TXT] = std::to_string(gridWidth);
	textBoxValues[GRID_SIZE_Y_TXT] = std::to_string(gridHeight);
	textBoxValues[SCREEN_SIZE_X_TXT] = std::to_string(windowWidth);
	textBoxValues[SCREEN_SIZE_Y_TXT] = std::to_string(windowHeight);
	textBoxValues[RANDOMIZE_PERCENT_TXT] = "50.0";
}

int Menu::CheckMenuPress(int mouseX, int mouseY)
{
	for (int i = OPEN_MENU_BTN; i < NUM_BUTTON_TYPE; i++)
	{
		if (mouseX >= buttons[i].x && mouseX <= buttons[i].x + buttons[i].w)
		{
			if (mouseY >= buttons[i].y && mouseY <= buttons[i].y + buttons[i].h)
			{
				return i;
			}
		}
	}
	for (int i = GRID_SIZE_X_TXT; i < NUM_TXT_TYPE; i++)
	{
		if (mouseX >= textBoxes[i].x && mouseX <= textBoxes[i].x + textBoxes[i].w)
		{
			if (mouseY >= textBoxes[i].y && mouseY <= textBoxes[i].y + textBoxes[i].h)
			{
				return i + NUM_BUTTON_TYPE;
			}
		}
	}
	return -1; //returns negative if no buttons were pressed
}

int Menu::HandleTextInput(SDL_Event event, int focusIndex)
{
	if (focusIndex < 0 || focusIndex >= NUM_TXT_TYPE)
		return -1;

	SDL_StartTextInput();

	if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == SDLK_BACKSPACE && textBoxValues[focusIndex].length() > 0)
			textBoxValues[focusIndex].pop_back();
		else if (event.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL)
			SDL_SetClipboardText(textBoxValues[focusIndex].c_str());
		else if (event.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL)
			textBoxValues[focusIndex] = SDL_GetClipboardText();
	}
	else if (event.type == SDL_TEXTINPUT)
	{
		textBoxValues[focusIndex].append(event.text.text);
	}

	//SDL_StopTextInput();

	SDL_Delay(100);

	return 1;
}

int Menu::WindowResized(int newWidth, int newHeight)
{
	windowWidth = newWidth;
	windowHeight = newHeight;
	CreateButtons();
	CreateTextBoxes();
	return 1;
}

int Menu::GridResized(int newWidth, int newHeight)
{
	gridWidth = newWidth;
	gridHeight = newHeight;
	
	textBoxValues[GRID_SIZE_X_TXT] = std::to_string(newWidth);
	textBoxValues[GRID_SIZE_Y_TXT] = std::to_string(newHeight);

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

int Menu::GetNumTextBoxes()
{
	return NUM_TXT_TYPE;
}

SDL_Rect *Menu::GetTextBoxes()
{
	return textBoxes;
}

std::string Menu::GetTextBoxValue(int textBoxNum)
{
	return textBoxValues[textBoxNum];
}
