#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include "player.h"
#include "map.h"
#include "bomb.h"

Player::Player(const char** bitmap, int i)
{
	this->x = 56;
	this->y = 104;

	if (i == 1)
	{
		this->x = 56;
		this->y = 104;
	}
	else if (i == 2)
	{
		this->x = 824;
		this->y = 584;
	}
	this->speed = 1;
	this->bombs = 0;
	this->maxBombs = 2;
	this->live = true;
	this->bitmap = al_load_bitmap(*bitmap);
}

void Player::draw()
{
	al_draw_bitmap(this->bitmap, this->x, this->y, 0);
}

void Player::reset(int i)
{
	if (i == 1)
	{
		this->x = 56;
		this->y = 104;
	}
	else if (i == 2)
	{
		this->x = 824;
		this->y = 584;
	}
	this->speed = 1;
	this->bombs = 0;
	this->maxBombs = 2;
	this->live = true;
}

bool Player::moveUP(Map map)
{
	for (int i = 0; i < map.x; i++)
	{
		for (int j = 0; j < map.y; j++)
		{
			// jezeli pole mapy nie jest puste, nie jest ogniem i nie jest bomb¹ tego gracza to nastepuje sprawdzenie kolizji.
			if (map.map[i][j] != 0 && map.map[i][j] != 5 && map.map[i][j] != 6)
			{
				// sprawdza czy wspolzedna x gracza znaduje sie w miejscu gdzie jest obiekt przez ktory nie moze przejsc
				if (this->x >= j * 48 && this->x <= j * 48 + 48)
				{
					// sprawdza czy wspolzedna y gracza znaduje sie w miejscu gdzie jest obiekt przez ktory nie moze przejsc
					if (this->y >= i * 48 + 47 && this->y <= i * 48 + 97)
					{
						return false;// jesli warunki zostana spelnione funkcja zwraca wartosc false co zablokuje mozliwosc ruchu w danym kierunku.
					}
				}
			}
			// sprawdzanie kolizji z ogniem
			else if (map.map[i][j] == 5)
			{
				if (x >= j * 48 && x <= j * 48 + 48)
				{
					if (y >= i * 48 + 47 && y <= i * 48 + 97)
					{
						//jesli kolizja nastepuje to zmienna lose nadaje wartosc true temu graczowi ktory przegral
						this->live = false;
						//umozliwa wejscie w ogien
						this->y -= this->speed;
					}
				}
			}
		}
	}
	//analogiczne sprawdzenie dla rogu po drugiej stronie.
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 19; j++)
		{
			if (map.map[i][j] != 0 && map.map[i][j] != 5 && map.map[i][j] != 6)
			{
				if (this->x + 32 >= j * 48 && this->x + 32 <= j * 48 + 48)
				{
					if (this->y >= i * 48 + 47 && this->y <= i * 48 + 97)
					{
						return false;
					}
				}
			}
		}
	}
	this->y -= this->speed;
	return true;
}

bool Player::moveDOWN(Map map)
{
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 19; j++)
		{
			if (map.map[i][j] != 0 && map.map[i][j] != 5  && map.map[i][j] != 6)
			{
				if (this->x >= j * 48 && this->x <= j * 48 + 48)
				{
					if (this->y - 16 >= i * 48 - 1 && this->y - 16 <= i * 48 - 1)
					{
						return false;
					}
				}
			}
			else if (map.map[i][j] == 5)
			{
				if (x >= j * 48 && x <= j * 48 + 48)
				{
					if (y >= i * 48 + 16 && y <= i * 48 + 64)
					{
						this->live = false;
						this->y += this->speed;
					}
				}
			}
		}
	}
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 19; j++)
		{
			if (map.map[i][j] != 0 && map.map[i][j] != 5 && map.map[i][j] != 6)
			{
				if (this->x + 32 >= j * 48 && this->x + 32 <= j * 48 + 48)
				{
					if (this->y - 16 >= i * 48 - 1 && this->y - 16 <= i * 48 - 1)
					{
						return false;
					}
				}
			}
		}
	}
	this->y += this->speed;
	return true;
}

bool Player::moveLEFT(Map map)
{
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 19; j++)
		{
			if (map.map[i][j] != 0 && map.map[i][j] != 5 && map.map[i][j] != 6)
			{
				if (x >= j * 48 - 1 && x <= j * 48 + 49)
				{
					if (y >= i * 48 + 48 && y <= i * 48 + 96)
					{
						return false;
					}
				}
			}
			else if (map.map[i][j] == 5)
			{
				if (x >= j * 48 - 1 && x <= j * 48 + 49)
				{
					if (y >= i * 48 + 48 && y <= i * 48 + 96)
					{
						this->live = false;
						this->x -= this->speed;
					}
				}
			}
		}
	}
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 19; j++)
		{
			if (map.map[i][j] != 0 && map.map[i][j] != 5 && map.map[i][j] != 6)
			{
				if (x >= j * 48 - 1 && x <= j * 48 + 49)
				{
					if (y + 32 >= i * 48 + 48 && y + 32 <= i * 48 + 96)
					{
						return false;
					}
				}
			}
		}
	}
	this->x -= this->speed;
	return true;
}
bool Player::moveRIGHT(Map map)
{
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 19; j++)
		{
			if (map.map[i][j] != 0 && map.map[i][j] != 5  && map.map[i][j] != 6)
			{
				if (this->x + 32 >= j * 48 - 1 && this->x + 32 <= j * 48 + 49)
				{
					if (this->y >= i * 48 + 48 && this->y <= i * 48 + 96)
					{
						return false;
					}
				}
			}
			else if (map.map[i][j] == 5)
			{
				if (this->x + 32 >= j * 48 - 1 && this->x + 32 <= j * 48 + 49)
				{
					if (this->y >= i * 48 + 48 && this->y <= i * 48 + 96)
					{
						this->live = false;
						this->x += this->speed;
					}
				}
			}
		}
	}
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 19; j++)
		{
			if (map.map[i][j] != 0 && map.map[i][j] != 5 && map.map[i][j] != 6)
			{
				if (x + 32 >= j * 48 - 1 && x + 32 <= j * 48 + 49)
				{
					if (y + 32 >= i * 48 + 48 && y + 32 <= i * 48 + 96)
					{
						return false;
					}
				}
			}
		}
	}
	this->x += this->speed;
	return true;
}