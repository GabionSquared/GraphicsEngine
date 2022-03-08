#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <list> //Linked list library for the renderqueue & activescripts
#include "Lists.h"
#include "GameObjectTypes.h"

using namespace NameSpace;

class RenderQueueManager {

	std::list<Renderable*> Queue;

public:
	RenderQueueManager() {

	}

	void Push(Renderable* pl) {
		Queue.push_front(pl);
		//adds to the front of the list
	}

	void Pop() {
		Queue.pop_front();
	}

	void Render() {

		SDL_RenderClear(Renderer);

		/*The holy rectangle of debugging
		SDL_Rect fillRect = { screenWidth / 4, screenHeight / 4, screenWidth / 2, screenHeight / 2 };
		SDL_SetRenderDrawColor(Renderer, 0xFF, 0x00, 0x00, 0xFF);
		SDL_RenderFillRect(Renderer, &fillRect);
		std::cout << "rendering Square\n";
		*/

		for (auto const& i : Queue) {
			SDL_RenderCopy(Renderer, i->GetText(), NULL, i->GetDest());
			//std::cout << "Rendering at " << i->getDestVerbose() << std::endl;
		}

		SDL_RenderPresent(Renderer);

	}
};
RenderQueueManager RQM;

class ActiveScriptManager {

	std::list<ActiveScript*> Queue;

public:
	ActiveScriptManager() {

	}

	void Push(ActiveScript* pl) {
		Queue.push_front(pl);
		//adds to the front of the list
	}

	void Pop() {
		Queue.pop_front();
	}

	void Process(const Uint8* currentKeyStates, double deltaTime) {


		for (auto const& i : Queue) {
			i->ProcessEvents(currentKeyStates, deltaTime);
		}

	}
};
ActiveScriptManager ASM;