#pragma once

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include "map.h"

class Player
{
public:
	float x;
	float y;
	int speed;
	int bombs;
	int maxBombs;
	bool live;
	ALLEGRO_BITMAP* bitmap = NULL;

	Player(const char** bitmap, int i);
	void draw();
	bool moveUP(Map map);
	bool moveDOWN(Map map);
	bool moveLEFT(Map map);
	bool moveRIGHT(Map map);
	void reset(int i);
};