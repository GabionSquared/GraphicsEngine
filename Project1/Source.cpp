#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "GameObjectTypes.h"
#include "Lists.h"

#pragma region Compiler Debugging
//https://lazyfoo.net/tutorials/SDL/01_hello_SDL/windows/msvc2019/index.php

//SDL2.dll not found
// 
// STEP 1:
//		CHECK THE DLL IS ACTUALLY THERE
//		C:\Users\40139037\source\repos\GabionSquared\GraphicsEngine\Project1\SDL2\lib\x64
// 
//		if it isn't, download it again
//			https://www.libsdl.org/download-2.0.php
//			https://www.libsdl.org/projects/SDL_image/
// 
// STEP 2:
//		FIX PATH
// 
//		windows key -> Edit Enviroment Variables For Your Account -> Path (under system variables) -> Edit -> New ->
//		C:\Users\40139037\source\repos\GabionSquared\GraphicsEngine\Project1\SDL2\lib\x64
//this is an awful way of doing this

//cannot open SDL2.lib
//
//check the lib file & folder is actually there. sometimes it vanishes.

//The application was unable to start correctly (0x000007b)
//
//Make sure you're launching from Project1.vcxproj under Debug|x64

//Can't see the full project list
//
//got to folder view in solution explorer (has just the folder and the .sln) and double click on the .sln
#pragma endregion Compiler Debugging

SDL_Surface* winSurface = NULL;
SDL_Window* Window = NULL;
SDL_Renderer* Renderer = NULL;
int screenWidth = 1280;
int screenHeight = 720;
int targetFPS = 60;
int allowedFrameTicks = 1000 / targetFPS;

int Init() {
	// Initialize SDL. SDL_Init will return -1 if it fails.
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
		system("pause");
		// End the program

		return 1;
	}

	// Create our window
	Window = SDL_CreateWindow("Why Does Nothing Work", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);

	// Make sure creating the window succeeded
	if (!Window) {
		std::cout << "Error creating window: " << SDL_GetError() << std::endl;
		system("pause");
		// End the program
		return 1;
	}

	Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
	if (!Renderer)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return 1;
	}

	//SDL_SetRenderDrawColor(Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_SetRenderDrawColor(Renderer, 100, 100, 100, 0xFF);
	//background colour

	// Get the surface from the window
	winSurface = SDL_GetWindowSurface(Window);

	// Make sure getting the surface succeeded
	if (!winSurface) {
		std::cout << "Error getting surface: " << SDL_GetError() << std::endl;
		system("pause");
		// End the program
		return 1;
	}

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
	}

	return -1;
}

SDL_Texture* LoadTexture(std::string path)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(Renderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}
	SDL_RenderCopy(Renderer, newTexture, NULL, NULL);

	//SDL_DestroyTexture( Texture );

	return newTexture;
}

class Timer {
	int startTicks;
	int pausedTicks;
	bool paused;
	bool started;

public:
	Timer() {
		startTicks = 0;
		pausedTicks = 0;
		paused = false;
		started = false;
	}

	void Start() {
		started = true;
		paused = false;
		startTicks = SDL_GetTicks(); //time since SDL was initialised
		pausedTicks = 0;
	}

	void Stop() {
		Timer();
	}

	void Pause() {
		if (started && !paused) {
			paused = true;

			pausedTicks = SDL_GetTicks() - startTicks;
			//essentially saves the position the timer was at

			startTicks = 0;
		}
	}
	void Unpause() {
		if (started && paused) {
			paused = false;

			startTicks = SDL_GetTicks() - pausedTicks;
			//essentially saves the position the timer was at

			startTicks = 0;
		}
	}

	//Say if you start the timer when SDL_GetTicks() reports 5000 ms and then you pause it at 10000ms.
	//This means the relative time at the time of pausing is 5000ms. If we were to unpause it when
	//SDL_GetTicks was at 20000, the new start time would be 20000 - 5000ms or 15000ms.
	//This way the relative time will still be 5000ms away from the current SDL_GetTicks time.

	int GetTicks() {
		//thousandths of a second

		if (started) {
			if (paused) {
				return pausedTicks;
			}
			else {
				return SDL_GetTicks() - startTicks;
			}
		}
		else {
			return -1;
		}
	}

	bool IsPaused() {
		return paused;
	}
	bool IsStarted() {
		return started;
	}
};

int Kill() {
	// Update the window display
	SDL_UpdateWindowSurface(Window);

	// Wait
	system("pause");

	// Destroy the window. This will also destroy the surface
	SDL_DestroyWindow(Window);
	SDL_DestroyRenderer(Renderer);
	Window = NULL;
	Renderer = NULL;

	// Quit SDL
	IMG_Quit();
	SDL_Quit();

	// End the program
	return 0;
}

int main(int argc, char** args) {

	Init();

	SDL_FillRect(winSurface, NULL, SDL_MapRGB(winSurface->format, 255, 255, 255));

	//-----------------------------------framerate things-------------------------------------------------

	Timer FramerateCalculator;	//how long game has been loaded
	Timer FramerateCapper;		//how long frame took to render
	FramerateCalculator.Start();
	int framesPassed = 0;
	float AvgFPS;
	bool showFPS = false;
	//-----------------------------------code goes in here-------------------------------------------------
	Moveable* plr = new Moveable();
	RQM.Push(plr);
	ASM.Push(plr);

	SDL_Event ev;
	bool running = true;
	// Main loop
	while (running) {
		// Event loop

		double deltaTime = (double)FramerateCapper.GetTicks()/1000;
		//std::cout << "deltaTime: " << deltaTime << std::endl;
		FramerateCapper.Start();

		while (SDL_PollEvent(&ev) != 0) {
			//User requests quit
			if (ev.type == SDL_QUIT)
			{
				running = false;
			}
		}

		/// &ev DOES have keyboard events in it, so can be used for taking keys.
		/// It's like if you were using a notepad: there's a delay when you hold it and it returns the digial key
		/// GetKeyboardState is more literal: just a load of booleans, no delays, no values. ignores keyboard type (qwerty, dvorak, etc).


		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		//list of scancodes: https://wiki.libsdl.org/SDL_Scancode

		ASM.Process(currentKeyStates, deltaTime);

		int x, y;
		Uint32 buttons = SDL_GetMouseState(&x, &y);
		//SDL_Log("Mouse cursor is at %d, %d", x, y);

		RQM.Render();

		framesPassed++;
		AvgFPS = framesPassed / (FramerateCalculator.GetTicks() / 1000.f);
		if (showFPS) {std::cout << framesPassed << " | " << FramerateCalculator.GetTicks() << " | " << (FramerateCalculator.GetTicks() / 1000.f) << " | " << AvgFPS << std::endl;}

		int frameTicks = FramerateCapper.GetTicks();
		if (frameTicks < allowedFrameTicks) {
			SDL_Delay(allowedFrameTicks - frameTicks);
		}
	}
	
	//-----------------------------------------------------------------------------------------------------

	Kill();
	return 0;
}