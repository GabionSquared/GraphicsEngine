#pragma once
#include <SDL.h>
#include <iostream>
#include "Object.h"
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
-------------------------------------------------------------------------*/
#pragma warning( disable : 4018 ) // '<': signed / unsigned mismatch

class GameManager {
int bricks;
public:
	SDL_Window* window;		//current window
	SDL_Surface* screen;	//visible screen surface
	bool running;			//is the game running or not?

	vector<Object*> drawList;	//list of all drawable objects in game
	vector<Object*> playerList;	//list of all player objects in game
	vector<Object*> ballList;	//list of all ball objects in game
	vector<Object*> brickList;	//list of all brick objects in game

	//constructor
	GameManager(int width, int height) {
		SDL_Init(SDL_INIT_EVERYTHING);
		window = SDL_CreateWindow("BrickBreaker", 50, 50, width, height, SDL_WINDOW_SHOWN);
		screen = SDL_GetWindowSurface(window);
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

			//update game data
			update();

			//redraw screen
			SDL_FillRect(screen, NULL, 0x000000);
			drawLevel();
			SDL_UpdateWindowSurface(window);
		}
	}

	void quit() {
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

	void addToDrawList(Object* o) { drawList.push_back(o); }
	void addToPlayerList(Object* o) { playerList.push_back(o); }
	void addToBallList(Object* o) { ballList.push_back(o); }
	void addToBrickList(Object* o) { brickList.push_back(o); }

	void update() {
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

	void drawLevel() {

		for (int i = 0; i < drawList.size(); i++) {
			drawList.at(i)->draw(window, screen);
		}
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
			return true;
		}
		else {
			return false;
		}
	}

	//ySpeed = -ySpeed
};
