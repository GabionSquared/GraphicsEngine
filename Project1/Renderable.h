#pragma once
#include <string>
#include <SDL.h>
#include <SDL_image.h>

class Renderable {
public:
	SDL_Rect* dest;
	SDL_Texture* Texture;
	SDL_Renderer* r;

	Renderable(std::string  filename, SDL_Renderer* Renderer, int x, int y);


	SDL_Rect* GetDest();
	std::string GetDestVerbose();
	void SetDest(int x, int y);
	SDL_Texture* GetText();
	void SetText(SDL_Texture* tex);
};