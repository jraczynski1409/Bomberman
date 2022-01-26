#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <time.h>
#include <list>
#include "player.h"
#include "map.h"
#include "bomb.h"
#include "fire.h"

using namespace std;

int main(void)
{
	// szerokoœæ i wysokoœæ ekranu
	const int WIDTH = 912;
	const int HEIGHT = 672;

	// inicjacja Allegro
	if (!al_init())
		return -1;

	// inicjacja rozszerzen allegro
	al_init_image_addon();

	// klawisze
	enum KEYS { W, S, A, D, SPACE, UP, DOWN, LEFT, RIGHT, RCTRL, ENTER, ESCAPE };
	bool keys[12] = { false, false, false, false, false, false, false, false, false, false, false, false };

	enum STATE { PLAYING, SCORE };
	int state = PLAYING;
	const int FPS = 180;
	bool done = false;
	bool end = false;
	bool redraw = true;
	bool redlose = false;
	bool bluelose = false;

	// zmienne wyswietlania, kolejki, timera
	ALLEGRO_DISPLAY* display = NULL;
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	ALLEGRO_TIMER* timer = NULL;

	// bitmapy
	ALLEGRO_BITMAP* redWin = NULL;
	ALLEGRO_BITMAP* blueWin = NULL;
	ALLEGRO_BITMAP* draw = NULL;

	//tworzenie okna programu
	display = al_create_display(WIDTH, HEIGHT);

	//sprawdzanie czy uda³o sie stworzyæ okno
	if (!display)
		return -1;

	// nadanie nazwy dla okna
	al_set_window_title(display, "Bomberman");

	// dodawanie sterownika klawiatury do programu
	al_install_keyboard();
	
	const char* tmp1 = "images/player1.png";
	Player player1(&tmp1, 1);
	const char* tmp2 = "images/player2.png";
	Player player2(&tmp2, 2);

	Map map;
	map.generate();

	list<Bomb> bombs;
	list<Fire> fires;

	redWin = al_load_bitmap("images/redWin.png");
	blueWin = al_load_bitmap("images/blueWin.png");
	draw = al_load_bitmap("images/draw.png");

	// tworzenie kolejki zdarzeñ
	event_queue = al_create_event_queue();

	ALLEGRO_EVENT event;

	// ograniczanie FPS-ów
	timer = al_create_timer(1.0 / FPS);

	// tworzenie rejestrow zdarzen
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	al_start_timer(timer);

	while (1)
	{
		al_wait_for_event(event_queue, &event);

		if (bombs.size() > 0)
		{
			if (bombs.front().putTime <= al_get_time() - 3)
			{
				int row = bombs.front().row;
				int coll = bombs.front().coll;

				fires.push_back(Fire(row, coll));
				map.map[row][coll] = 5;

				if (map.map[row - 1][coll] == 2)
				{
					fires.push_back(Fire(row - 1, coll));
					map.map[row - 1][coll] = 5;
				}
				else if (map.map[row - 1][coll] == 0)
				{
					fires.push_back(Fire(row - 1, coll));
					map.map[row - 1][coll] = 5;
					if (map.map[row - 2][coll] == 0 || map.map[row - 2][coll] == 2)
					{
						fires.push_back(Fire(row - 2, coll));
						map.map[row - 2][coll] = 5;
					}
				}
				if (map.map[row + 1][coll] == 2)
				{
					fires.push_back(Fire(row + 1, coll));
					map.map[row + 1][coll] = 5;
				}
				else if (map.map[row + 1][coll] == 0)
				{
					fires.push_back(Fire(row + 1, coll));
					map.map[row + 1][coll] = 5;
					if (map.map[row + 2][coll] == 0 || map.map[row + 2][coll] == 2)
					{
						fires.push_back(Fire(row + 2, coll));
						map.map[row + 2][coll] = 5;
					}
				}
				if (map.map[row][coll - 1] == 2)
				{
					fires.push_back(Fire(row, coll - 1));
					map.map[row][coll - 1] = 5;
				}
				else if (map.map[row][coll - 1] == 0)
				{
					fires.push_back(Fire(row, coll - 1));
					map.map[row][coll - 1] = 5;
					if (map.map[row][coll - 2] == 0 || map.map[row][coll - 2] == 2)
					{
						fires.push_back(Fire(row, coll - 2));
						map.map[row][coll - 2] = 5;
					}
				}
				if (map.map[row][coll + 1] == 2)
				{
					fires.push_back(Fire(row, coll + 1));
					map.map[row][coll + 1] = 5;
				}
				if (map.map[row][coll + 1] == 0)
				{
					fires.push_back(Fire(row, coll + 1));
					map.map[row][coll + 1] = 5;
					if (map.map[row][coll + 2] == 0 || map.map[row][coll + 2] == 2)
					{
						fires.push_back(Fire(row, coll + 2));
						map.map[row][coll + 2] = 5;
					}
				}
				switch (bombs.front().owner)
				{
				case 1:
					player1.bombs--;
					break;
				case 2:
					player2.bombs--;
					break;
				}
				bombs.front().detonate(player1, player2, map);
				bombs.pop_front();
			}
		}

		if (fires.size() > 0)
		{
			if (fires.front().putTime <= al_get_time() - 3)
			{
				int row = fires.front().row;
				int coll = fires.front().coll;
				map.map[row][coll] = 0;
				fires.pop_front();
			}
		}

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			return 0;
		}

		// zdarzenia ktore trwaja caly czas podczas wciskania przyciskow
		else if (event.type == ALLEGRO_EVENT_TIMER)
		{
			redraw = true;

			// gracz 1
			if (keys[W])
			{
				player1.moveUP(map);
			}
			if (keys[S])
			{
				player1.moveDOWN(map);
			}
			if (keys[A])
			{
				player1.moveLEFT(map);
			}
			if (keys[D])
			{
				player1.moveRIGHT(map);
			}

			// gracz 2
			if (keys[UP])
			{
				player2.moveUP(map);
			}
			if (keys[DOWN])
			{
				player2.moveDOWN(map);
			}
			if (keys[LEFT])
			{
				player2.moveLEFT(map);
			}
			if (keys[RIGHT])
			{
				player2.moveRIGHT(map);
			}
		}
		else if (event.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (event.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				keys[ESCAPE] = true;
				break;
			case ALLEGRO_KEY_ENTER:
				keys[ENTER] = true;
				break;

			case ALLEGRO_KEY_W:
				keys[W] = true;
				break;
			case ALLEGRO_KEY_S:
				keys[S] = true;
				break;
			case ALLEGRO_KEY_A:
				keys[A] = true;
				break;
			case ALLEGRO_KEY_D:
				keys[D] = true;
				break;
			case ALLEGRO_KEY_SPACE:
				if (player1.bombs < player1.maxBombs)
				{
					int row = (player1.y - 16) / 48;
					int coll = (player1.x + 16) / 48;
					if (map.map[row][coll] != 6)
					{
						player1.bombs++;
						map.map[row][coll] = 6;
						bombs.push_back(Bomb(1, row, coll));
					}
				}
				break;

			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_RCTRL:
				if (player2.bombs < player2.maxBombs)
				{
					int row = (player2.y - 16) / 48;
					int coll = (player2.x + 16) / 48;
					if (map.map[row][coll] != 6)
					{
						player2.bombs++;
						map.map[row][coll] = 6;
						bombs.push_back(Bomb(2, row, coll));
					}
				}
				break;
			}
		}
		else if (event.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (event.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				keys[ESCAPE] = false;
				break;
			case ALLEGRO_KEY_ENTER:
				keys[ENTER] = false;
				break;
			case ALLEGRO_KEY_W:
				keys[W] = false;
				break;
			case ALLEGRO_KEY_S:
				keys[S] = false;
				break;
			case ALLEGRO_KEY_A:
				keys[A] = false;
				break;
			case ALLEGRO_KEY_D:
				keys[D] = false;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = false;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			}
		}
		if (state == PLAYING)
		{
			if (!player1.live && !player2.live)
			{
				state = SCORE;
				map.draw();
				player1.draw();
				player2.draw();
				al_draw_bitmap(draw, WIDTH / 2 - 200, HEIGHT / 2 - 100, 0);
				redraw = true;
			}
			else if (!player1.live)
			{
				state = SCORE;
				map.draw();
				player1.draw();
				player2.draw();
				al_draw_bitmap(blueWin, WIDTH / 2 - 200, HEIGHT / 2 - 100, 0);
				redraw = true;
			}
			else if (!player2.live)
			{
				state = SCORE;
				map.draw();
				player1.draw();
				player2.draw();
				al_draw_bitmap(redWin, WIDTH / 2 - 200, HEIGHT / 2 - 100, 0);
				redraw = true;
			}
		}
		if (redraw && al_is_event_queue_empty(event_queue))
		{
			if (state == PLAYING)
			{
				map.draw();
				player1.draw();
				player2.draw();
				al_flip_display();// wyswietlenie tego na ekranie
				al_clear_to_color(al_map_rgb(0, 150, 50)); // nadanie koloru tla na zielony
				redraw = false;
			}
			else if (state == SCORE)
			{
				al_flip_display();
				if (keys[ENTER])
				{
					state = PLAYING;
					map.generate();
					player1.reset(1);
					player2.reset(2);
					bombs.clear();
					fires.clear();
					redraw = true;
				}
				if (keys[ESCAPE])
				{
					break;
				}
			}
		}
	}
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);

	al_destroy_bitmap(redWin);
	al_destroy_bitmap(blueWin);
	al_destroy_bitmap(draw);
	al_destroy_display(display);

	return 0;
}