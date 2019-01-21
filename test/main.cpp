#include "rinput.h"

#include <iostream>

bool quit = false;
int main()
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow(
		"SDL2Test",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		0
		);

	SDL_Event iEvent;

	RInput::Init(window);

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	RInput::RegisterAction("moveforward", KEYBOARD_W, GAMEPAD_BUTTON_LSTICK_UP, true);
	RInput::RegisterAction("movebackward", KEYBOARD_S, GAMEPAD_BUTTON_LSTICK_DOWN, true);
	RInput::RegisterAction("moveleft", KEYBOARD_A, GAMEPAD_BUTTON_LSTICK_LEFT, true);
	RInput::RegisterAction("moveright", KEYBOARD_D, GAMEPAD_BUTTON_LSTICK_RIGHT, true);
	RInput::RegisterAction("jump", KEYBOARD_SPACE, GAMEPAD_BUTTON_A, false);

	RInput::LoadActionsFromFile("controller.xml");

	while (!quit)
	{
		while (SDL_PollEvent(&iEvent) != 0)
		{
			if (iEvent.type == SDL_QUIT)
			{
				quit = true;
			}

			RInput::TestEvents(iEvent);
		}

		if (RInput::GetActionInput(RInput::GetAction("moveleft")))
		{
			printf("Moving Left!\n");
		}

		if (RInput::GetActionInput(RInput::GetAction("moveright")))
		{
			printf("Moving Right!\n");
		}

		if (RInput::GetActionInput(RInput::GetAction("moveforward")))
		{
			printf("Moving up!\n");
		}

		if (RInput::GetActionInput(RInput::GetAction("movebackward")))
		{
			printf("Moving down!\n");
		}

		if (RInput::GetActionInput(RInput::GetAction("jump")))
		{
			printf("Jump!\n");
		}

		//TestKeyboard();
		//TestMouse();
		//TestGamePad();
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}