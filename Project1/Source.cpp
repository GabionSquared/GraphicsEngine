#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <list> //Linked list library for the renderqueue

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

double TowardZero(double num, double strength) {
	return num == 0 || (num < strength ? num*-1 : num) < strength ? 0 : num > strength ? num - strength : num + strength;
	//if num = 0 or abs(num) < str, return 0. num > str, return num-str. else, return num+str.
}

double TowardZeroD(double num, double strength) {
	double abs = num;
	if (num < 0) {
		abs = num * -1;
	}
	std::cout << "abs: " << abs <<"| str: "<< strength << "\n";
	if (abs < strength || num == 0) {
		std::cout << "return 0 \n";
		return 0;
	}
	if (num < strength) {
		std::cout << "return num+str \n";
		return num + strength;
	}
	if (num > strength) {
		std::cout << "return num-str \n";
		return num - strength;
	}
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

class Renderable {
protected:
	SDL_Rect* dest = new SDL_Rect;
	SDL_Texture* Texture;

public:
	SDL_Rect* GetDest() {
		return dest;
	}

	std::string GetDestVerbose() {
		return "x: " + std::to_string(dest->x) + "   y: " + std::to_string(dest->y);
	}

	void SetDest(int x, int y) {
		dest->x = x;
		dest->y = y;
	}

	SDL_Texture* GetText() {
		return Texture;
	}

	void SetText(SDL_Texture* tex) {
		Texture = tex;
		SDL_Point size;
		SDL_QueryTexture(tex, NULL, NULL, &size.x, &size.y);
		dest->w = size.x;
		dest->h = size.y;
		//std::cout << "width: " << dest->w;
	}

	void Update() {

	}

	Renderable(int x = 100, int y = 50) {

		//std::string path = R"(C:\Users\40139037\source\repos\GabionSquared\GraphicsEngine\Project1\Sunkist.png)";
		std::string path = R"(Sunkist.png)";

		SetText(LoadTexture(path));

		SetDest(x, y);
	}
};

class Moveable : public Renderable {
protected:
	int velocityX;
	int velocityY;

	int maxVelocityX;
	int maxVelocityY;

	bool boarderCap;
	bool boarderMirroring;
	bool isMirror;
	bool hasMirror;

public:
	Moveable(int x = 100, int y = 50, bool mirror = false) : Renderable(x, y) {
		velocityX = 0;
		velocityY = 0;

		maxVelocityX = 40;
		maxVelocityY = 40;
		/// This is attached to the std::round in Move()
		/// The problem with this is we currently can't move less than 1px a frame
		/// need a function that can decide to rythmically drop frames.

		boarderCap = false;
		boarderMirroring = true;
		isMirror = mirror;
		hasMirror = false;
	}

	void Move(double deltaTime) {

		//std::cout << "velX: " << velocityX << " | DeltaTime: " << deltaTime << " | Product: " << velocityX * deltaTime << " | New Position: " << dest->x + (velocityX * deltaTime) << std::endl;
		//std::cout << "velY: " << velocityY << " | DeltaTime: " << deltaTime << " | Product: " << velocityY * deltaTime << " | New Position: " << dest->y + (velocityY * deltaTime) << std::endl;

		dest->x = std::round(dest->x + (velocityX * deltaTime));
		dest->y = std::round(dest->y + (velocityY * deltaTime));

		if (boarderCap) {
			//std::cout << (dest->x < 0) << (dest->x + dest->w > screenWidth);
			if ((dest->x <= 0) || (dest->x + dest->w >= screenWidth)) {
				//std::cout << "at boundary X\n";
				dest->x -= velocityX * deltaTime;
				velocityX = 0;
			}
			if ((dest->y <= 0) || (dest->y + dest->h >= screenHeight)) {
				//std::cout << "at boundary Y\n";
				dest->y -= velocityY * deltaTime;
				velocityY = 0;
			}
		}

		if (boarderMirroring || !isMirror) {
			if ((dest->x <= 0) || (dest->x + dest->w >= screenWidth) || !hasMirror) {
				Moveable* plr = new Moveable(screenWidth,dest->h,true);
			}
			if ((dest->y <= 0) || (dest->y + dest->h >= screenHeight) || !hasMirror) {
				Moveable* plr = new Moveable(dest->w, screenHeight, true);
			}

			if (dest->x+dest->w < 0 || dest->x > screenWidth || dest->y + dest->h < 0 || dest->y > screenHeight ) {
				RQM.Pop();
				delete this;
			}
		}
		
	}

	void ProcessEvents(const Uint8* currentKeyStates, double deltaTime) {

		if (currentKeyStates[SDL_SCANCODE_UP]    || currentKeyStates[SDL_SCANCODE_W])
		{
			velocityY -= maxVelocityY;
		}
		if (currentKeyStates[SDL_SCANCODE_DOWN]  || currentKeyStates[SDL_SCANCODE_S])
		{
			velocityY += maxVelocityY;
		}
		if (currentKeyStates[SDL_SCANCODE_LEFT]  || currentKeyStates[SDL_SCANCODE_A])
		{
			velocityX -= maxVelocityX;
		}
		if (currentKeyStates[SDL_SCANCODE_RIGHT] || currentKeyStates[SDL_SCANCODE_D])
		{
			velocityX += maxVelocityX;
		}

		/* CHECK EVENT LOOP IF YOU FORGET WHY THIS IS CRINGE
		* 
		//remember this function is called INSIDE a for loop iterating through events
		//if key was pressed but wasn't pressed last frame
		if (ev.type == SDL_KEYDOWN) {

			switch (ev.key.keysym.sym) {
			case SDLK_UP:
			case SDLK_w:
				velocityY += maxVelocityY; break;
			case SDLK_DOWN:
			case SDLK_s:
				velocityY -= maxVelocityY; break;
			case SDLK_LEFT:
			case SDLK_a:
				velocityX += maxVelocityX; break;
			case SDLK_RIGHT:
			case SDLK_d:
				velocityX -= maxVelocityX; break;
			}
		}

		//if key was released
		if (ev.type == SDL_KEYUP) {

			switch (ev.key.keysym.sym) {
				case SDLK_UP:
				case SDLK_w:
					velocityY -= maxVelocityY; break;
				case SDLK_DOWN:
				case SDLK_s:
					velocityY += maxVelocityY; break;
				case SDLK_LEFT:
				case SDLK_a:
					velocityX -= maxVelocityX; break;
				case SDLK_RIGHT:
				case SDLK_d:
					velocityX += maxVelocityX; break;
			}
		}
		*/

		Update(deltaTime);
	}

	void Update(double deltaTime) {
		Move(deltaTime);

		velocityX = TowardZero(velocityX, 10);
		velocityY = TowardZero(velocityY, 10);
	}

	void CreateMirror() {

	}
};

class RenderQueueManager {

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
	Timer FramerateCalculator;	//how long game has been loaded
	Timer FramerateCapper;		//how long frame took to render
	FramerateCalculator.Start();
	int framesPassed = 0;
	float AvgFPS;
	bool showFPS = false;
	//-----------------------------------code goes in here-------------------------------------------------
	Moveable* plr = new Moveable();
	RQM.Push(plr);

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
		/// GetKeyboardState is more literal: just a load of booleans, no delays, no values. ignores keyboard type.


		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		//list of scancodes: https://wiki.libsdl.org/SDL_Scancode

		plr->ProcessEvents(currentKeyStates, deltaTime);

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