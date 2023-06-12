#pragma once
#include "Object.h"

/*-----------------------------------------------------------------
	Player class
	Derived from the Object class.
	The Player is a drawable object which uses its own rectangle
	(SDL_Rect position) to draw a filled rectangle. It does not load
	any graphics.
	The Player class implements the Object interface (so contains its own
	versions of the move() and react() functions()).
-----------------------------------------------------------------*/
class Player : public Object {
	vector<Vertex> Verticies() {
		// Define the vertices of the cubeoid
		vector<Vertex> vertices;
		vertices.push_back({ -1.0f, -1.0f, -1.0f }); // Front bottom left
		vertices.push_back({ 1.0f, -1.0f, -1.0f });  // Front bottom right
		vertices.push_back({ 1.0f, 1.0f, -1.0f });   // Front top right
		vertices.push_back({ -1.0f, 1.0f, -1.0f });  // Front top left
		vertices.push_back({ -1.0f, -1.0f, 1.0f });  // Back bottom left
		vertices.push_back({ 1.0f, -1.0f, 1.0f });   // Back bottom right
		vertices.push_back({ 1.0f, 1.0f, 1.0f });    // Back top right
		vertices.push_back({ -1.0f, 1.0f, 1.0f });   // Back top left

		//vector<Vertex>* _vertices = & vertices; //compiler was being weird
		return vertices;
	}

public:
	int id;
	vector<Vertex> model;

	//default constructor
	Player() {
		GenerateVBO();
		position.x = 0;
		position.y = 0;
		position.w = 20;
		position.h = 100;
		id = 1;
	}

	
	//overloaded constructor allows alteration of position, size and id of the player
	Player(int x, int y, int w, int h, int tid) {
		position.x = x;
		position.y = y;
		position.w = w;
		position.h = h;
		id = tid;
	}

	void react(char m) {
		if (m == 'g') {
			active = false;
		}
		if (m == 'r') {
			position.x = (800 / 2) - 50;
		}
	}

	void update() {

	}

	/*-------------------------------------------------------------------------
		move()
		The move function defines controls and movement restrictions for the
		player paddles. The player paddle cannot leave the top or bottom of
		the screen.
	-------------------------------------------------------------------------*/
	void move() {
		const Uint8* key = SDL_GetKeyboardState(NULL);

		if (id == 1 && active) {

			if (position.x > 0) {
				if (key[SDL_SCANCODE_W] || key[SDL_SCANCODE_A] || key[SDL_SCANCODE_UP] || key[SDL_SCANCODE_LEFT]) {
					position.x--;
				}
			}

			if (position.x < (800 - position.w)) {
				if (key[SDL_SCANCODE_S] || key[SDL_SCANCODE_D] || key[SDL_SCANCODE_DOWN] || key[SDL_SCANCODE_RIGHT]) {
					position.x++;
				}
			}
		}

		//if (!active) {
		//	cout << "player is disabled" << endl;
		//}
	}
};

