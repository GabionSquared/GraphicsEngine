#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>

class Renderable {

public:
	//vars
	SDL_Rect* dest;
	SDL_Texture* Texture;
	SDL_Renderer* Renderer;

	//funcs
	SDL_Rect* GetDest();
	std::string GetDestVerbose();
	void SetDest(int x, int y);
	SDL_Texture* GetText();
	void SetText(SDL_Texture* tex);
	Renderable(std::string  filename, SDL_Renderer* r, int x, int y);

	SDL_Texture* LoadTexture(std::string path);
};

/// <summary>
/// the purpose of this class is to be overwitten by other shit, but it needed a normalised place to be held for the script processing queue
/// </summary>
class ActiveScript {
public:
	ActiveScript();
	virtual void ProcessEvents(const Uint8* currentKeyStates, double deltaTime);
	virtual void Kill();
};

class Moveable : public Renderable, public ActiveScript {
protected:
	int velocityX;
	int velocityY;

	int maxVelocityX;
	int maxVelocityY;

	bool blockBoarderMovement;
	bool mirrorsAtBoarders;
	bool isMirror;
	bool hasMirror;
	bool isInGameZone;

	float boarder;
	float defaultBoarder;

	std::string thisfilename;

public:
	Moveable(std::string filename, int x = 100, int y = 50, bool mirror = false, int velX = 40, int velY = 40) : Renderable(filename, Renderer, x, y);
	virtual void Update(double deltaTime);
	virtual void ProcessEvents(const Uint8* currentKeyStates, double deltaTime) override;
	void Kill();
};
