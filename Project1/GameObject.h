#pragma once
#include "Renderable.h"

class GameObject : public Renderable {
protected:
	std::string thisfilename;


public:
	GameObject(std::string filename, SDL_Renderer* Renderer, int x, int y);
	virtual void Update(double deltaTime);
	virtual void ProcessEvents(const Uint8 * currentKeyStates, double deltaTime);
	void Kill();
};