#include <iostream>
#include <SDL.h>
#include "GameManager.h"
#include "Player.h"
#include "Ball.h"
#include "Brick.h"
#include "Menu.h"

using namespace std;

/*
	take my word for it this works when sdl is in the mood to
	fuction properly and not deleting it's .lib for no reason
	
	CONTROLS

		W/A/UP/LEFT
			Move left

		S/D/DOWN/RIGHT
				Move left

		ESC
			Close the game

		R
			Reset the game (moves the player and ball to the starting position)
*/


int main(int argc, char** argv) {
	int SCREEN_WIDTH = 800;
	int SCREEN_HEIGHT = 500;

	//resources
	GameManager manager(SCREEN_WIDTH, SCREEN_HEIGHT);


	//                     x                    y           w    h   tid
	Player player((SCREEN_WIDTH/2)-50, (SCREEN_HEIGHT-40), 100, 20, 1);
	Ball ball(500, 300, 20, 20, 1);
	Menu menu(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
	manager.addToMenuList(&menu);

	Brick bricks[40];
	
	int brickID = 0;

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 10; x++)
		{
			bricks[brickID].position.x = ((SCREEN_WIDTH / static_cast<double>(10)) * x) + 2.;
			bricks[brickID].position.y = 20 * y + 5;
			bricks[brickID].id = brickID; 
			
			manager.addToDrawList(&bricks[brickID]);
			manager.addToBrickList(&bricks[brickID]);

			brickID++;
		}
	}
	

	manager.addToDrawList(&player);
	manager.addToDrawList(&ball);

	manager.addToPlayerList(&player);
	manager.addToBallList(&ball);

	//main game loop
	manager.run();

	//shutdown
	manager.quit();

	return 0;
}