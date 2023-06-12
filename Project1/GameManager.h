#pragma once
#include <SDL.h>
#include <iostream>
#include "Object.h"
#include <SDL_mixer.h>
#include <vector>
using namespace std;

/*-------------------------------------------------------------------------
	GameManager class
	The GameManager class handles the overall running of the game including:
	- initialising, setup and shutdown of SDL (graphics system)
	- drawing of objects
	- collision checking
	- message passing

	The GameManager maintains a set of lists of all in game objects and uses
	these to manage the resources and states in game.

	https://sketchfab.com/3d-models/fat-among-us-crewate-f1f2591472584d27b8fb697931214fd5
-------------------------------------------------------------------------*/
#pragma warning( disable : 4018 ) // '<': signed / unsigned mismatch
enum GState { splash, title, game };

class GameManager {
int bricks;
GState gameState = splash;
bool statebuffer = false;

public:
	SDL_Window* window;		//current window
	SDL_Surface* screen;	//visible screen surface
	bool running;			//is the game running or not?

	vector<Object*> drawList;	//list of all drawable objects in game
	vector<Object*> playerList;	//list of all player objects in game
	vector<Object*> ballList;	//list of all ball objects in game
	vector<Object*> brickList;	//list of all brick objects in game
	Object* menu = nullptr;

	//The music that will be played
	Mix_Music* music = nullptr;

	//The sound effects that will be used
	Mix_Chunk* soundBounce = nullptr;
	Mix_Music* soundMusic = nullptr;

	//constructor
	GameManager(int width, int height) {
		SDL_Init(SDL_INIT_EVERYTHING);
		window = SDL_CreateWindow("BrickBreaker", 50, 50, width, height, SDL_WINDOW_SHOWN);
		screen = SDL_GetWindowSurface(window);
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
		soundBounce = Mix_LoadWAV("Bruh.wav");
		soundMusic = Mix_LoadMUS("Wendy's Training Video Hot Drinks.wav");
		running = true;
		bricks = 40;
	}

	/*---------------------------------------------------------------------
		run()
		location of the main game loop
	---------------------------------------------------------------------*/
	void run() {
		SDL_Event e;
		//main game loop
		while (running) {
			//take user input
			while (SDL_PollEvent(&e)) {
				if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
					running = false;
				}
			}

			switch (gameState)
			{
				case splash: updateSplash(); drawSplash(); break;
				case title:  updateTitle();  drawTitle();  break;
				case game:   updateGame();   drawGame();   break;
			}
		}
	}

	void quit() {
		//sound
		Mix_FreeChunk(soundBounce);
		Mix_FreeMusic(soundMusic);
		Mix_CloseAudio();

		SDL_FreeSurface(screen);
		SDL_DestroyWindow(window);
	}

	void notify(char m) {
		cout << "Sending notify" << endl;

		//GAMEOVER
		//freezes the game
		if (m == 'g') {
			for (int i = 0; i < playerList.size(); i++) {
				playerList.at(0)->react('g');
			}
			for (int i = 0; i < ballList.size(); i++) {
				ballList.at(0)->react('g');
			}
		}

		//RESET (centers ball and paddle. doesn't reset bricks, but i have
		//no clue how to do that anyway because they aren't being deleted.)
		if (m == 'r') {

			for (int i = 0; i < playerList.size(); i++) {
				playerList.at(0)->react('r');
			}
			for (int i = 0; i < ballList.size(); i++) {
				ballList.at(0)->react('r');
			}
		}
	}

	void addToDrawList  (Object* o) { drawList  .push_back(o); }
	void addToPlayerList(Object* o) { playerList.push_back(o); }
	void addToBallList  (Object* o) { ballList  .push_back(o); }
	void addToBrickList (Object* o) { brickList .push_back(o); }
	void addToMenuList  (Object* o) { menu = o; }

	void updateSplash() {
		//pressing enter-> title screen
		const Uint8* key = SDL_GetKeyboardState(NULL);
		if (key[SDL_SCANCODE_RETURN] || key[SDL_SCANCODE_RETURN2]) {
			if (!statebuffer) {
				statebuffer = true;
				gameState = title;
			}
		}
		else {
			statebuffer = false;
		}
	}
	void updateTitle() {
		//pressing enter -> game
		const Uint8* key = SDL_GetKeyboardState(NULL);
		if (key[SDL_SCANCODE_RETURN] || key[SDL_SCANCODE_RETURN2]) {
			if (!statebuffer) {
				statebuffer = true;
				Mix_HaltMusic();
				gameState = game;
			}
		}
		else {
			statebuffer = false;
		}
		if (Mix_PlayingMusic() == 0)
		{
			Mix_PlayMusic(soundMusic, -1);
		}
	}
	void updateGame() {
		Mix_HaltMusic();
		//win game -> splash screen
		//lose game->title screen

		for (int i = 0; i < playerList.size(); i++) {
			playerList.at(i)->move();
		}

		for (int i = 0; i < ballList.size(); i++) {
			ballList.at(i)->move();
		}

		//collisions
		if (checkCollision(playerList.at(0), ballList.at(0))) {
			ballList.at(0)->react('b');
		}

		for (int i = 0; i < brickList.size(); i++) {
			if (checkCollision(brickList.at(i), ballList.at(0))) {
				ballList.at(0)->react('b');
				brickList.at(i)->react('_');
				bricks--;
			}
		}

		if (bricks = 0 || ballList.at(0)->position.y + ballList.at(0)->position.h >= 500) {
			notify('g');
		}

		//check for reset
		const Uint8* key = SDL_GetKeyboardState(NULL);

		if (key[SDL_SCANCODE_R]) {
			notify('r');
		}
	}

	void drawSplash() {
		//rotating logo
		SDL_FillRect(screen, NULL, 0xFFD041);
		SDL_UpdateWindowSurface(window);
	}
	void drawTitle() {
		//title image
		SDL_FillRect(screen, NULL, 0x88F800);
		menu->draw(window, screen);
		SDL_UpdateWindowSurface(window);
	}
	void drawGame() {

		SDL_FillRect(screen, NULL, 0x000000);
		for (int i = 0; i < drawList.size(); i++) {
			drawList.at(i)->draw(window, screen);
		}
		SDL_UpdateWindowSurface(window);
	}

	bool checkCollision(Object* o1, Object* o2){
		int left1 =   o1->position.x;
		int right1 =  o1->position.x + o1->position.w;
		int top1 =    o1->position.y;
		int bottom1 = o1->position.y + o1->position.h;

		int left2 =   o2->position.x;
		int right2 =  o2->position.x + o2->position.w;
		int top2 =    o2->position.y;
		int bottom2 = o2->position.y + o2->position.h;

		if (left1 < right2 && right1 > left2 && top1 < bottom2 && bottom1 > top2) {
			Mix_PlayChannel(-1, soundBounce, 0);
			return true;
		}
		else {
			return false;
		}
	}

};

