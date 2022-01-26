#include "bomb.h"
#include "map.h"

Bomb::Bomb(int owner, int row, int coll)
{
	this->owner = owner;
	this->row = row;
	this->coll = coll;
	this->putTime = al_get_time();
}

void Bomb::detonate(Player &player1, Player &player2, Map map)
{
	int x = player1.x;
	int y = player1.y;

	//sprawdzamy czy w miejscu gdzie jest gracz wybuchla bomba po przeciwleg³ych bokach.
	if (map.map[y / 48 - 1][x / 48] == 5)
	{
		player1.live = false;
	}
	if (map.map[(y + 32) / 48 - 1][(x + 32) / 48] == 5)
	{
		player1.live = false;
	}

	x = player2.x;
	y = player2.y;

	if (map.map[y / 48 - 1][x / 48] == 5)
	{
		player2.live = false;
	}
	if (map.map[(y + 32) / 48 - 1][(x + 32) / 48] == 5)
	{
		player2.live = false;
	}
}