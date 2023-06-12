#pragma once
#include <SDL.h>
#include <iostream>
#include <vector>

#include <glew.h>
#include <SDL_opengl.h>
using namespace std;

//we make our 3d models manually :gigachad:
struct Vertex {
	float x, y, z;
};

/*-------------------------------------------------------------------
	Object class:
	Base class for all in game objects.
	Any elements of the game to be used should be inherited (derived)
	from Object.
	Object contains the base drawing function for all objects so any
	elements based on this will know how to draw automatically.
-------------------------------------------------------------------*/
class Object {
protected:

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

	void GenerateVBO() {
		vector<Vertex> Vertices = Verticies();
		//compiler was (still) being weird. who cares, it's local. Is this a memory leak? probably /shrug

		glGenBuffers(1, &vbo);

		// Bind the VBO
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		// Upload the vertex data to the VBO
		glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), Vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glEnableVertexAttribArray(0);
	}

public:
	//variables
	SDL_Surface* image;		//holds bmp image if one is loaded.
	SDL_Rect	position;	//holds x, y, w and h of object
	bool active;

	GLuint vbo; //vertex buffer object

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
