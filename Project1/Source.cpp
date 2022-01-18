#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <string>

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
//		windows key -> Edit the System Enviroment Variables -> Path (under system variables) -> Edit -> New ->
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
		dest.y += yDif;
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
	window = SDL_CreateWindow("SuperDogSimulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN);

	// Make sure creating the window succeeded
	if (!window) {
		cout << "Error creating window: " << SDL_GetError() << endl;
		system("pause");
		// End the program
		return 1;
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

	//-----------------------------------code goes in here-------------------------------------------------

	Player plr;
	plr.Init();

	SDL_Event ev;
	bool running = true;
	// Main loop
	while (running) {
		// Event loop
		while (SDL_PollEvent(&ev) != 0) {
			// check event type
			switch (ev.type) {
			case SDL_QUIT:
				Kill();
				running = false;
				break;

			case SDL_KEYDOWN:
				// test keycode
				switch (ev.key.keysym.sym) {
				case SDLK_w:
					plr.Move(10, 0);

					break;
				case SDLK_s:
					plr.Move(-10, 0);
					break;
					// etc
				}
				case SDLK_a:
					plr.Move(0, 10);

					break;
				case SDLK_d:
					plr.Move(0, -10);
					break;
					// etc
				}
			}
		}

		// Wait before next frame
		SDL_Delay(16.7); //(60 fps in ms)
		// Draw the next frame
		SDL_UpdateWindowSurface(window);

	
	//-----------------------------------------------------------------------------------------------------

	Kill();
	return 0;
}
