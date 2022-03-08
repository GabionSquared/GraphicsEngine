#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <string>

namespace NameSpace {
    double TowardZero(double num, double strength);
    double TowardZeroD(double num, double strength);

    class Renderable
    {
    protected:
        SDL_Rect* dest;
        SDL_Texture* Texture;

    public:
        SDL_Rect* GetDest();
        std::string GetDestVerbose();
        void SetDest(int x, int y);
        SDL_Texture* GetText();
        void SetText(SDL_Texture* tex);
        Renderable(int x = 100, int y = 50);

    };

    class ActiveScript
    {
    public:
        ActiveScript();
        virtual void ProcessEvents(const Uint8* currentKeyStates, double deltaTime);
    };

    class Moveable
    {
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
        Moveable(int x = 100, int y = 50, bool mirror = false);
        void Move(double deltaTime);
        void ProcessEvents(const Uint8* currentKeyStates, double deltaTime);
        void Update(double deltaTime);
        void CreateMirror();
        void Kill();

    };
}