#pragma once
#include <SDL.h>
#include <iostream>
using namespace std;

/*-------------------------------------------------------------------
	Object class:
	Base class for all in game objects.
	Any elements of the game to be used should be inherited (derived)
	from Object.
	Object contains the base drawing function for all objects so any
	elements based on this will know how to draw automatically.
-------------------------------------------------------------------*/
class Object {
public:

	//variables
	SDL_Surface* image;		//holds bmp image if one is loaded.
	SDL_Rect		position;	//holds x, y, w and h of object

	bool active;

	//functions
	Object() {	//constructor
		position.x = position.y = position.w = position.h = 0;
		image = nullptr;
		active = true;
	}

	// Virtual functions will need to be implemented in any derived classes.
	virtual void move() = 0;	//pure virtual function for moving object
	virtual void react(char m) = 0;	//pure virtual function for object reactions to in game events
	virtual void update() = 0;	//pure virtual function for object reactions to in game events

	/*---------------------------------------------------------------------
		draw()
		Accepts the current window and screen surface to draw onto.
		The function will draw an image from the object if one is loaded.
		If no image is loaded then the draw function will fill the defined
		rectangle (position) for the object.
	----------------------------------------------------------------------*/
	void draw(SDL_Window* window, SDL_Surface* screen) {
		if (image == nullptr) {
			SDL_FillRect(screen, &position, 0x00ff00);
			//SDL_UpdateWindowSurface(window);
		}
		else {
			SDL_BlitSurface(image, &position, screen, nullptr);
			//SDL_UpdateWindowSurface(window);
		}
	}

};

