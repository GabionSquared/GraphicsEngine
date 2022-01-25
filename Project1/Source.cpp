#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <string>

//https://lazyfoo.net/tutorials/SDL/01_hello_SDL/windows/msvc2019/index.php
// 
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

//The application was unable to start correctly (0x000007b)
//
//Make sure you're launching from Project1.vcxproj under Debug|x64

//Can't see the full project list
//
//got to foldier view in solution explorer (has just the folder and the .sln) and double click on the .sln



using namespace std;

SDL_Surface* winSurface = NULL;
SDL_Window* window = NULL;
SDL_Renderer* Renderer = NULL;
int screenWidth = 1280;
int screenHeight = 720;

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

	int getTicks() {
		//thousandths of a second

		if (started) {
			if (paused) {
				return pausedTicks;
			}
			else {
				return SDL_GetTicks() - startTicks;
			}
		}
	}

	bool isPaused() {
		return paused;
	}
	bool isStarted() {
		return started;
	}
};

class Player {
	SDL_Rect dest;
	SDL_Surface* image;

	public:
	void Init() {
		//image = SDL_LoadBMP("Sunkist.bmp");

		string path = R"(C:\Users\40139037\source\repos\GabionSquared\GraphicsEngine\Project1\Sunkist.png)";

		image = IMG_Load(path.c_str());

		if (!image) {
			// load failed

			cout << "Failed to load dog" << endl;
		}

		dest.x = 100;
		dest.y = 50;

		int result = SDL_BlitSurface(image, NULL, winSurface, &dest);

		/*
		dest.x = 160;
		dest.y = 120;
		dest.w = 320;
		dest.h = 240;
		SDL_BlitScaled(image, NULL, winSurface, &dest);
		*/
	}

	void Move(int xDif, int yDif) {

		dest.x += xDif;
		dest.y -= yDif;
		int result = SDL_BlitSurface(image, NULL, winSurface, &dest);
	}

};

int Init() {
	// Initialize SDL. SDL_Init will return -1 if it fails.
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "Error initializing SDL: " << SDL_GetError() << endl;
		system("pause");
		// End the program

		return 1;
	}

	// Create our window
	window = SDL_CreateWindow("SuperDogSimulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);

	// Make sure creating the window succeeded
	if (!window) {
		cout << "Error creating window: " << SDL_GetError() << endl;
		system("pause");
		// End the program
		return 1;
	}
	else {

	}

	// Get the surface from the window
	winSurface = SDL_GetWindowSurface(window);

	// Make sure getting the surface succeeded
	if (!winSurface) {
		cout << "Error getting surface: " << SDL_GetError() << endl;
		system("pause");
		// End the program
		return 1;
	}

	int imgFlags = IMG_INIT_PNG;

	return -1;
}

int Kill() {
	// Update the window display
	SDL_UpdateWindowSurface(window);

	// Wait
	system("pause");

	// Destroy the window. This will also destroy the surface
	SDL_DestroyWindow(window);

	// Quit SDL
	SDL_Quit();

	// End the program
	return 0;
}

// You must include the command line parameters for your main function to be recognized by SDL
int main(int argc, char** args) {

	Init();

	SDL_FillRect(winSurface, NULL, SDL_MapRGB(winSurface->format, 255, 255, 255));

	//framerate things
	Timer FramerateTimer;
	FramerateTimer.Start();
	int framesPassed = 0;
	float AvgFPS;

	//-----------------------------------code goes in here-------------------------------------------------

	Player plr;
	plr.Init();

	SDL_Event ev;
	bool running = true;
	// Main loop
	while (running) {
		// Event loop
		while (SDL_PollEvent(&ev) != 0) {
			//User requests quit
			if (ev.type == SDL_QUIT)
			{
				running = false;
			}
		}

		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		//list of scancodes: https://wiki.libsdl.org/SDL_Scancode

		if (currentKeyStates[SDL_SCANCODE_UP] || currentKeyStates[SDL_SCANCODE_W])
		{
			plr.Move(0, 10);
		}
		else if (currentKeyStates[SDL_SCANCODE_DOWN] || currentKeyStates[SDL_SCANCODE_S])
		{
			plr.Move(0, -10);
		}
		if (currentKeyStates[SDL_SCANCODE_LEFT] || currentKeyStates[SDL_SCANCODE_A])
		{
			plr.Move(-10, 0);
		}
		else if (currentKeyStates[SDL_SCANCODE_RIGHT] || currentKeyStates[SDL_SCANCODE_D])
		{
			plr.Move(10, 0);
		}


		//SDL_Delay(16.7);

		// Draw the next frame
		SDL_UpdateWindowSurface(window);
		framesPassed++;
		AvgFPS = framesPassed / (FramerateTimer.getTicks() / 1000.f);
		cout << framesPassed <<" | " << FramerateTimer.getTicks() <<" | " << (FramerateTimer.getTicks() / 1000.f) <<" | " << AvgFPS << endl;
	}
	
	//-----------------------------------------------------------------------------------------------------

	Kill();
	return 0;
}
