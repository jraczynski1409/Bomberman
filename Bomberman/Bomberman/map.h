#pragma once

#include <allegro5/allegro.h>

class Map
{
public:
	int x;
	int y;
	int map[13][19];

	ALLEGRO_BITMAP* wall = al_load_bitmap("images/unbreakable.png");
	ALLEGRO_BITMAP* brick = al_load_bitmap("images/brick.png");
	ALLEGRO_BITMAP* bomb = al_load_bitmap("images/bomb.png");
	ALLEGRO_BITMAP* fire = al_load_bitmap("images/fire.png");
	ALLEGRO_BITMAP* control = al_load_bitmap("images/control.png");
	
	void generate();
	void draw();
};

