#include "GameObject.h"

//This exists to be overridden. Don't think about it too hard
//ActiveScript is dead, long live GameObject.

GameObject::GameObject(std::string filename, SDL_Renderer* Renderer, int x, int y) : Renderable(filename, Renderer, x, y) {
	thisfilename = filename;

}

void GameObject::Update(double deltaTime) {

}

void GameObject::ProcessEvents(const Uint8* currentKeyStates, double deltaTime)  {

	Update(deltaTime);
}

void GameObject::Kill() {

	delete this;
};
