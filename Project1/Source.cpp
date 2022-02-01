#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <list> //for the renderqueue

#pragma region Compiler Debugging
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

//cannot open SDL2.lib
//
//check the lib file & folder is actually there. sometimes it vanishes.

//The application was unable to start correctly (0x000007b)
//
//Make sure you're launching from Project1.vcxproj under Debug|x64

//Can't see the full project list
//
//got to foldier view in solution explorer (has just the folder and the .sln) and double click on the .sln
#pragma endregion Compiler Debugging

SDL_Surface* winSurface = NULL;
SDL_Window* Window = NULL;
SDL_Renderer* Renderer = NULL;
int screenWidth = 1280;
int screenHeight = 720;

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

	SDL_SetRenderDrawColor(Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
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

	return -1;
}

SDL_Texture* loadTex(std::string path) {

	SDL_Texture* finalTex = NULL;

	SDL_Surface* loadingSurf = IMG_Load(path.c_str());
	//path needs to be turned into a character list... i think?

	if (!loadingSurf) {
		std::cout << "Error getting surface: " << SDL_GetError() << std::endl;
	}
	else {
		//use the renderer to turn the surface into a texture
		finalTex = SDL_CreateTextureFromSurface(Renderer, loadingSurf);

		if (!finalTex) {
			std::cout << "Error creating texture from surface: " << SDL_GetError() << std::endl;
		}

		SDL_FreeSurface(loadingSurf);
	}
	return finalTex;
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
		else {
			return -1;
		}
	}

	bool isPaused() {
		return paused;
	}
	bool isStarted() {
		return started;
	}
};

class Renderable {
	SDL_Rect* dest = new SDL_Rect;
	SDL_Texture* Texture;

public:
	SDL_Rect* getDest() {
		return dest;
	}

	std::string getDestVerbose() {
		return "x: " + std::to_string(dest->x) + "   y: " + std::to_string(dest->y);
	}

	void setDest(int x, int y) {
		dest->x = x;
		dest->y = y;
	}

	SDL_Texture* getText() {
		return Texture;
	}

	void setText(SDL_Texture* tex) {
		Texture = tex;
	}

	Renderable() {

		std::string path = R"(C:\Users\40139037\source\repos\GabionSquared\GraphicsEngine\Project1\Sunkist.png)";
		Texture = loadTex(path);

		setDest(100, 50);

		//int result = SDL_BlitSurface(image, NULL, winSurface, &dest);
	}

	void Move(int xDif, int yDif) {

		dest->x += xDif;
		dest->y -= yDif;
	}

};

static class RenderQueueManager {

	std::list<Renderable*> Queue;

public:
	RenderQueueManager() {

	}

	void Push(Renderable* pl) {
		Queue.push_front(pl);
		//adds to the front of the list
	}

	void Pop() {
		Queue.pop_front();
	}

	void Render() {

		SDL_RenderClear(Renderer);

		SDL_Rect fillRect = { screenWidth / 4, screenHeight / 4, screenWidth / 2, screenHeight / 2 };
		SDL_SetRenderDrawColor(Renderer, 0xFF, 0x00, 0x00, 0xFF);
		SDL_RenderFillRect(Renderer, &fillRect);

		for (auto const& i : Queue) {
			SDL_RenderCopy(Renderer, i->getText(), NULL, i->getDest());
			std::cout << "Rendering at " << i->getDestVerbose() << std::endl;
		}

		SDL_RenderPresent(Renderer);

	}
};
RenderQueueManager RQM;

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

	//framerate things
	Timer FramerateTimer;
	FramerateTimer.Start();
	int framesPassed = 0;
	float AvgFPS;
	bool showFPS = false;
	//-----------------------------------code goes in here-------------------------------------------------
	Renderable* plr = new Renderable();
	RQM.Push(plr);

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
			plr->Move(0, 10);
		}
		else if (currentKeyStates[SDL_SCANCODE_DOWN] || currentKeyStates[SDL_SCANCODE_S])
		{
			plr->Move(0, -10);
		}
		if (currentKeyStates[SDL_SCANCODE_LEFT] || currentKeyStates[SDL_SCANCODE_A])
		{
			plr->Move(-10, 0);
		}
		else if (currentKeyStates[SDL_SCANCODE_RIGHT] || currentKeyStates[SDL_SCANCODE_D])
		{
			plr->Move(10, 0);
		}


		//SDL_Delay(16.7);

		// Draw the next frame
		RQM.Render();

		framesPassed++;
		AvgFPS = framesPassed / (FramerateTimer.getTicks() / 1000.f);
		if (showFPS) {std::cout << framesPassed << " | " << FramerateTimer.getTicks() << " | " << (FramerateTimer.getTicks() / 1000.f) << " | " << AvgFPS << std::endl;}
	}
	
	//-----------------------------------------------------------------------------------------------------

	Kill();
	return 0;
}