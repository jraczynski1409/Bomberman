#pragma once

#include <allegro5/allegro.h>
#include "player.h"
#include "map.h"

class Bomb
{
public:
	int owner;
	int row;
	int coll;
	int putTime;

	Bomb(int owner, int row, int coll);
	void detonate(Player &player1, Player &player2, Map map);
};
