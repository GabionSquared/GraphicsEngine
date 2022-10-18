#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <list> //Linked list library for the renderqueue & activescripts
#include <ctime> //for srand seeds, inside ball spawning

#include "Timer.h"
#include "GameObject.h"


#pragma region Compiler Debugging
//https://lazyfoo.net/tutorials/SDL/index.php

//SDL2.dll not found
// 
// STEP 1:
//		CHECK THE DLL IS ACTUALLY THERE
//		C:\Users\40139037\source\repos\GabionSquared\GraphicsEngine\Project1\SDL2
// 
//		if it isn't, download it again
//			https://github.com/libsdl-org/SDL/releases/tag/release-2.24.0
//			https://github.com/libsdl-org/SDL_image/releases
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

static SDL_Surface* winSurface = NULL;
static SDL_Window* Window = NULL;
static SDL_Renderer* Renderer = NULL;
static int screenWidth = 1280;
static int screenHeight = 720;
static int targetFPS = 60;
static int allowedFrameTicks = 1000 / targetFPS;

#pragma warning( disable : 4244 )
// '=' conversion from 'double' to 'int', possible loss of data
#pragma warning( disable : 4267 )
// 'return' conversion from 'size_t' to 'int', possible loss of data

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

double TowardZero(double num, double strength) {
	return num == 0 || (num < strength ? num*-1 : num) < strength ? 0 : num > strength ? num - strength : num + strength;
	//if num = 0 or abs(num) < str, return 0. num > str, return num-str. else, return num+str.

	//this has started causing passive force and i dont know why
}

double TowardZeroD(double num, double strength) {

	double abs = num;

	//std::cout << TowardZero(num, strength) << std::endl;

	if (num < 0) {
		abs = num * -1;
	}
	//std::cout << "abs: " << abs <<"| str: "<< strength << "\n";
	if (abs < strength || num == 0) {
		//std::cout << "return 0 \n";
		return 0;
	}
	if (num < strength) {
		//std::cout << "return num+str \n";
		return num + strength;
	}
	if (num > strength) {
		//std::cout << "return num-str \n";
		return num - strength;
	}
	return -1;
}

class RenderQueueManager {

	std::list<Renderable*> Queue;

public:
	RenderQueueManager() {

	}

	void Push(Renderable* pl) {
		Queue.push_front(pl);
		//adds to the front of the list
	}

	void PopFront() {
		Queue.pop_front();
	}
	void PopBack() {
		Queue.pop_back();
	}

	int Count() {
		return Queue.size();
	}

	void Render() {

		SDL_RenderClear(Renderer);

		/*The holy rectangle of debugging
		SDL_Rect fillRect = { screenWidth / 4, screenHeight / 4, screenWidth / 2, screenHeight / 2 };
		SDL_SetRenderDrawColor(Renderer, 0xFF, 0x00, 0x00, 0xFF);
		SDL_RenderFillRect(Renderer, &fillRect);
		std::cout << "rendering Square\n";
		*/

		for (auto const& i : Queue) {
			SDL_RenderCopy(Renderer, i->GetText(), NULL, i->GetDest());
			//std::cout << "Rendering at " << i->getDestVerbose() << std::endl;
		}

		SDL_RenderPresent(Renderer);

	}
};
static RenderQueueManager RQM;

class ActiveScriptManager {

	std::list<GameObject*> Queue;

public:
	ActiveScriptManager() {

	}

	void Push(GameObject* pl) {
		Queue.push_front(pl);
		//adds to the front of the list
	}

	void PopFront() {
		Queue.pop_front();
	}
	void PopBack() {
		Queue.pop_back();
	}

	int Count() {
		return Queue.size();
	}

	void Process(const Uint8* currentKeyStates, double deltaTime) {
		for (auto const& i : Queue) {
			i->ProcessEvents(currentKeyStates, deltaTime);
		}

	}
};
static ActiveScriptManager ASM;

class ShadowRealm {

	std::list<GameObject*> Queue;

public:
	ShadowRealm() {

	}

	void Push(GameObject* pl) {
		Queue.push_front(pl);
		//adds to the front of the list
	}

	void Pop() {
		Queue.pop_front();
	}

	int Count() {
		return Queue.size();
	}

	void CleanUp(){

		//can only delete one thing a frame, but at least it works
		if (Count() > 0) {
			std::cout << "doing murder " << std::endl;
			Queue.front()->Kill();
			Pop();
		}

		/*
		int c = 1;

		for (auto const& i : Queue) {
			if (Count() == 0) {
				continue;
			}

			std::cout << "doing murder " << c << std::endl;
			c++;

			i->Kill();
			Pop();
		}
		*/
	}
};
static ShadowRealm SR;


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
	
	//Ball* ball = new Ball("Ball.png", 640, 360);
	GameObject* Dog = new GameObject("SunKist.png", Renderer, 640, 360);

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

		//SDL_Log("Renders: %d\tScripts: %d\tShadow Realm: %d", RQM.Count(), ASM.Count(), SR.Count());

		ASM.Process(currentKeyStates, deltaTime);

		int x, y;
		Uint32 buttons = SDL_GetMouseState(&x, &y);
		//SDL_Log("Mouse cursor is at %d, %d", x, y);

		RQM.Render();

		SR.CleanUp();

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