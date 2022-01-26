#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <time.h>
#include "map.h"

void Map::generate()
{
	srand(time(NULL));

	int los1;
	int los2;

	//0-wolne miejsca
	//1-niezniszczalne sciany
	//2-zniszczalna sciana
	//5-ogien bomby
	//6-bomba
	//9-miejsce gdzie nie moze byc sciany

	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 19; j++)
		{
			if (i == 0 || i == 12)
			{
				map[i][j] = 1;
			}
			else
			{
				if (i % 2 == 0)
				{
					if (j == 0 || j == 18)
					{
						map[i][j] = 1;
					}
					else if (j % 2 == 1)
					{
						map[i][j] = 0;
					}
					else
					{
						map[i][j] = 1;
					}
				}
				else
				{
					if (j == 0 || j == 18)
					{
						map[i][j] = 1;
					}
					else
					{
						map[i][j] = 0;
					}
				}
			}
		}
	}
	// miejsca w ktorych nie mozna postawic sciany
	map[1][1] = 9;
	map[2][1] = 9;
	map[1][2] = 9;

	map[10][17] = 9;
	map[11][17] = 9;
	map[11][16] = 9;

	int k = 0;

	// petla ktora losuje miejsca dla 100 scian zniszczalnych
	while (k < 100)
	{
		los1 = rand() % 13;
		los2 = rand() % 19;
		if (map[los1][los2] == 0)
		{
			map[los1][los2] = 2;
			k++;
		}
	}

	map[1][1] = 0;
	map[2][1] = 0;
	map[1][2] = 0;
	map[10][17] = 0;
	map[11][17] = 0;
	map[11][16] = 0;
}

void Map::draw()
{
	al_draw_bitmap(this->control, 0, 0, 0);
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 19; j++)
		{
			switch (this->map[i][j])
			{
			case 1:
				al_draw_bitmap(this->wall, j * 48, i * 48 + 48, 0);
				break;
			case 2:
				al_draw_bitmap(this->brick, j * 48, i * 48 + 48, 0);
				break;
			case 5:
				al_draw_bitmap(this->fire, j * 48, i * 48 + 48, 0);
				break;
			case 6:
			case 7:
				al_draw_bitmap(this->bomb, j * 48, i * 48 + 48, 0);
				break;
			}
		}
	}
}