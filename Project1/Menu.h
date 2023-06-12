#pragma once
#include "Object.h"

class Menu : public Object {
public:
	int id;

	//default constructor
	Menu() {
		position.x = 0;
		position.y = 0;
		position.w = 75;
		position.h = 15;
		id = 1;
		active = true;
	}


	//overloaded constructor allows alteration of position, size and id of the player
	Menu(int x, int y, int w, int h, int tid) {
		position.x = x;
		position.y = y;
		position.w = w;
		position.h = h;
		id = tid;
		active = true;
		image = SDL_LoadBMP("menu.bmp");
		//image = IMG_Load(path.c_str()) "menu.bmp";
	}

	void react(char m) {}
	void update() {}
	void move() {}
};

