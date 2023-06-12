#pragma once
#include "Object.h"

/*-----------------------------------------------------------------
	Enemy class
	Derived from the Object class.
	The Enemy is a drawable object which uses its own rectangle
	(SDL_Rect position) to draw a filled rectangle. It does not load
	any graphics.
	The Enemy class implements the Object interface (so contains its own
	versions of the move() and react() functions()).
-----------------------------------------------------------------*/
class Ball : public Object {
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
	int xSpeed, ySpeed;

	//constructor
	Ball() {
		GenerateVBO();
		position.x = 512;
		position.y = 350;
		position.w = 20;
		position.h = 20;
		id = 1;
		xSpeed = 1;
		ySpeed = 1;
	}

	//overloaded constructor allows alteration of position, size and id of the Enemy
	Ball(int x, int y, int w, int h, int tid) {
		position.x = x;
		position.y = y;
		position.w = w;
		position.h = h;
		id = tid;
		xSpeed = 1;
		ySpeed = -1;
	}

	void react(char m) {
		if (m == 'b') {
			ySpeed = -ySpeed;
		}
		if (m == 'g') {
			active = false;
		}
		if (m == 'r') {
			position.x = 500;
			position.y = 300;
		}
	}
	void update() {

	}

	/*-------------------------------------------------------------------------
		move()
		The move() function defines the conditions for the motion of the enemy
		(ball) object in game.
		The enemy direction is inverted if it hits any of the 4 walls (screen edge).
	-------------------------------------------------------------------------*/
	void move() {
		if (active) {
			position.x += xSpeed;
			position.y += ySpeed;

			if (position.x >= (800 - position.w)) {
				xSpeed = -xSpeed;
			}
			else if (position.x <= 0) {
				xSpeed = -xSpeed;
			}

			if (position.y >= (500 - position.h)) {
				ySpeed = -ySpeed;
			}
		}
		//if (!active) {
		//	cout << "ball is disabled" << endl;
		//}
	}
	
};
