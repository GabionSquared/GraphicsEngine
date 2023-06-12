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
class Brick : public Object {
	vector<Vertex> Verticies() {
		// Define the vertices of the cubeoid
		std::vector<Vertex> vertices;
		vertices.push_back({ -1.0f, -1.0f, -1.0f }); // Front bottom left
		vertices.push_back({ 1.0f, -1.0f, -1.0f });  // Front bottom right
		vertices.push_back({ 1.0f, 1.0f, -1.0f });   // Front top right
		vertices.push_back({ -1.0f, 1.0f, -1.0f });  // Front top left
		vertices.push_back({ -1.0f, -1.0f, 1.0f });  // Back bottom left
		vertices.push_back({ 1.0f, -1.0f, 1.0f });   // Back bottom right
		vertices.push_back({ 1.0f, 1.0f, 1.0f });    // Back top right
		vertices.push_back({ -1.0f, 1.0f, 1.0f });   // Back top left

		//vector<Vertex>* _vertices = &vertices; //compiler was being weird
		return vertices;
	}


public:
	int id;

	//default constructor
	Brick() {
		GenerateVBO();
		position.x = 0;
		position.y = 0;
		position.w = 75;
		position.h = 15;
		id = 1;
		active = true;
	}


	//overloaded constructor allows alteration of position, size and id of the player
	Brick(int x, int y, int w, int h, int tid) {
		position.x = x;
		position.y = y;
		position.w = w;
		position.h = h;
		id = tid;
		active = true;
	}

	void react(char m) {
		position.x = -1;
		position.y = -1;
		position.w = 1;
		position.h = 1;
	}

	void update() {

	}

	void move() {

	}
};

