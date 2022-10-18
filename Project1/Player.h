#pragma once

class Player{
public:
	int posX;
	int posY;
	int width;
	int height;
	int health;
	int speed;
	int spriteIndex;

	void Move();
	void Shoot();
	void CheckCollision();
};

