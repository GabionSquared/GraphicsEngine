#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "GameObjectTypes.h"
#include "Lists.h"

int screenWidth = 1280;
int screenHeight = 720;

double TowardZero(double num, double strength) {
	return num == 0 || (num < strength ? num * -1 : num) < strength ? 0 : num > strength ? num - strength : num + strength;
	//if num = 0 or abs(num) < str, return 0. num > str, return num-str. else, return num+str.
}

double TowardZeroD(double num, double strength) {
	double abs = num;
	if (num < 0) {
		abs = num * -1;
	}
	std::cout << "abs: " << abs << "| str: " << strength << "\n";
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
	return -1;
}

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

	Renderable(int x = 100, int y = 50) {

		//std::string path = R"(C:\Users\40139037\source\repos\GabionSquared\GraphicsEngine\Project1\Sunkist.png)";
		std::string path = R"(Sunkist.png)";

		SetText(LoadTexture(path));

		SetDest(x, y);
	}
};

class ActiveScript {
public:
	ActiveScript() {

	}

	virtual void ProcessEvents(const Uint8* currentKeyStates, double deltaTime) {

	}
};

class Moveable : public Renderable, public ActiveScript {
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
				Moveable* plr = new Moveable(screenWidth, dest->h, true);
			}
			if ((dest->y <= 0) || (dest->y + dest->h >= screenHeight) || !hasMirror) {
				Moveable* plr = new Moveable(dest->w, screenHeight, true);
			}

			if (dest->x + dest->w < 0 || dest->x > screenWidth || dest->y + dest->h < 0 || dest->y > screenHeight) {
				Kill();
			}
		}
	}

	void ProcessEvents(const Uint8* currentKeyStates, double deltaTime) override {

		if (currentKeyStates[SDL_SCANCODE_UP] || currentKeyStates[SDL_SCANCODE_W])
		{
			velocityY -= maxVelocityY;
		}
		if (currentKeyStates[SDL_SCANCODE_DOWN] || currentKeyStates[SDL_SCANCODE_S])
		{
			velocityY += maxVelocityY;
		}
		if (currentKeyStates[SDL_SCANCODE_LEFT] || currentKeyStates[SDL_SCANCODE_A])
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

	void Kill() {
		RQM.Pop();
		ASM.Pop();
		std::cout << "deleting self";
		delete this;
	}
};