#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <list>
#include "GameObjectTypes.h"

namespace NameSpace {
    class RenderQueueManager
    {
    private:
        std::list<Renderable*> Queue;

    public:
        RenderQueueManager();
        void Push(Renderable* pl);
        void Pop();
        void Render();
    };

    class ActiveScriptManager
    {
    private:
        std::list<ActiveScript*> Queue;

    public:
        ActiveScriptManager();
        void Push(ActiveScript* pl);
        void Pop();
        void Process(const Uint8* currentKeyStates, double deltaTime);
    };
}