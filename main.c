#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include "Game.h"


//funkcyjka do narysowania jednego gracza
//do zrobienia rysowanie nicku
//KLIENT
void drawPlayer(Player* player) {
	//rysowanie koła
	al_draw_filled_circle(player->posiotion.x, player->posiotion.y, PLAYER_SIZE, player->color);
	//twarda matma here - rysowanie strzaleczki
	al_draw_filled_triangle(player->posiotion.x + (PLAYER_SIZE / 5) * sin(-player->angle), player->posiotion.y + (PLAYER_SIZE / 5) * cos(-player->angle),
		player->posiotion.x + (PLAYER_SIZE * 6 / 7) * sin(-player->angle + M_PI * 45 / 180), player->posiotion.y + (PLAYER_SIZE * 6 / 7) * cos(-player->angle + M_PI * 45 / 180),
		player->posiotion.x - (PLAYER_SIZE * 4 / 5) * sin(player->angle + M_PI), player->posiotion.y + (PLAYER_SIZE * 4 / 5) * cos(player->angle + M_PI), al_map_rgb(0, 0, 0));
	al_draw_filled_triangle(player->posiotion.x + (PLAYER_SIZE / 5) * sin(-player->angle), player->posiotion.y + (PLAYER_SIZE / 5) * cos(-player->angle),
		player->posiotion.x - (PLAYER_SIZE * 6 / 7) * sin(player->angle + M_PI * 45 / 180), player->posiotion.y + (PLAYER_SIZE * 6 / 7) * cos(player->angle + M_PI * 45 / 180),
		player->posiotion.x - (PLAYER_SIZE * 4 / 5) * sin(player->angle + M_PI), player->posiotion.y + (PLAYER_SIZE * 4 / 5) * cos(player->angle + M_PI), al_map_rgb(0, 0, 0));
	//rysowanie
	float hpbarW = 28;
	float hpbarH = 4;
	al_draw_filled_rectangle(player->posiotion.x - hpbarW / 2, player->posiotion.y - PLAYER_SIZE - 5 - hpbarH, player->posiotion.x + hpbarW / 2, player->posiotion.y - PLAYER_SIZE - 5, al_map_rgb(0, 0, 0));
	al_draw_filled_rectangle(player->posiotion.x - hpbarW / 2, player->posiotion.y - PLAYER_SIZE - 5 - hpbarH, player->posiotion.x - hpbarW / 2 + hpbarW * player->health, player->posiotion.y - PLAYER_SIZE - 5, al_map_rgb(0, 200, 0));

}

//funkcja do rysowanja pociskow
//KLIENT
void drawBullet(Bullet* bullet) {
	al_draw_filled_circle(bullet->posiotion.x, bullet->posiotion.y, BULLET_SIZE, al_map_rgb(0, 0, 0));
}

//funkcyjka do rysowania calej planszy
//tutaj jeszcze trzeba bedzie dodac scoreboard i pociski itd
//KLIENT
void drawBoard(Board* board) {
	al_clear_to_color(al_map_rgb(20, 80, 150));
	for (int i = 0; i < board->playersAmt; i++) {
		drawPlayer(board->players[i]);
	}
	for (int i = 0; i < board->bulletAmt; i++) {
		drawBullet(board->bullets[i]);
	}
	al_flip_display();
}


//funkcja do tworzenia Player*
Player* initPlayer(const char* nick, int score, Cord position, float angle, ALLEGRO_COLOR color) {
	Player* p = (Player*)malloc(sizeof(Player));
	int i = 0;
	char c = nick[0];
	while (c != '\0' && i != 15) {
		p->nick[i] = nick[i];
		i++;
		c = nick[i];
	}
	p->posiotion = position;
	p->score = score;
	p->angle = angle;
	p->color = color;
	p->health = 1;
	return p;
}

Bullet* initBullet(Cord position, float angle, Player* shooter) {
	Bullet* b = (Bullet*)malloc(sizeof(Bullet));
	b->posiotion = position;
	b->angle = angle;
	b->shooter = shooter;
	return b;
}

void addPlayer(Player* player, Board* board) {
	board->players[board->playersAmt] = player;
	board->playersAmt++;
}
void addBullet(Bullet* bullet, Board* board) {
	board->bullets[board->bulletAmt] = bullet;
	board->bulletAmt++;
}

void shoot(Player* player, Board* board) {
	Bullet* bullet = initBullet(player->posiotion, player->angle, player);
	bullet->angle = M_PI - bullet->angle; //popraweczka znowu bo trygonometria cos nie pykala
	float x = sin(bullet->angle) * (PLAYER_SIZE + BULLET_SIZE + 1);
	float y = cos(bullet->angle) * (PLAYER_SIZE + BULLET_SIZE + 1);
	bullet->posiotion.x += x;
	bullet->posiotion.y += y;
	addBullet(bullet, board);
}


int main(int argc, char* argv[]) {

	//inicjalizacji w bród
	ALLEGRO_DISPLAY* display = NULL;

	al_init_primitives_addon();

	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	display = al_create_display(BOARD_SIZE_X, BOARD_SIZE_Y);
	if (!display) {
		fprintf(stderr, "failed to create display!\n");
		return -1;
	};

	const float FPS = 60;
	ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);

	ALLEGRO_EVENT_QUEUE* queue;
	queue = al_create_event_queue();

	al_install_keyboard();
	al_install_mouse();

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_mouse_event_source());
	al_register_event_source(queue, al_get_timer_event_source(timer));

	al_start_timer(timer);
	//koniec inicjalizacji

	al_clear_to_color(al_map_rgb(20, 80, 150));

	Cord c = { 350, 350 };
	Player* mainPlayer = initPlayer(" ", 0, c, 0, al_map_rgb(100, 0, 0));
	c = (Cord){ 100, 100 };
	Player* debil = initPlayer(" ", 0, c, 0, al_map_rgb(0, 100, 0));
	float angle = 0;
	bool running = true;
	bool keys[4] = { 0,0,0,0 };

	Board* board = initBoard();
	addPlayer(mainPlayer, board);
	addPlayer(debil, board);

	while (running) {
		ALLEGRO_EVENT event;

		drawBoard(board);
		al_wait_for_event(queue, &event);
		// to tutaj jest do zmiany - obliczenia beda wykonywane po stronie servera, a po kazdym evencie bedzie wysyłany sygnal
		// to jest wersja testowa w sumie
		if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
			float x = event.mouse.x - mainPlayer->posiotion.x;
			float y = event.mouse.y - mainPlayer->posiotion.y;
			float poprawka = M_PI;
			if (x >= 0) poprawka = 0;
			float angle = atan(y / x) + M_PI / 2 + poprawka; //no obliczenia cos nie wyszły, ale jakos sie załatalo
			mainPlayer->angle = angle;

		}
		else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			shoot(mainPlayer, board);
		}
		else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (event.keyboard.keycode) {
			case ALLEGRO_KEY_ESCAPE:
				running = false;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = 1;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = 1;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = 1;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = 1;
				break;
			}
		}
		else if (event.type == ALLEGRO_EVENT_KEY_UP) {
			switch (event.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				keys[UP] = 0;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = 0;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = 0;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = 0;
				break;
			}
		}
		if (event.type == ALLEGRO_EVENT_TIMER) {
			updateBullets(board);
			checkColisions(board);
			if (keys[LEFT])
				mainPlayer->posiotion.x -= PLAYER_SPEED;
			if (keys[RIGHT])
				mainPlayer->posiotion.x += PLAYER_SPEED;
			if (keys[DOWN])
				mainPlayer->posiotion.y += PLAYER_SPEED;
			if (keys[UP])
				mainPlayer->posiotion.y -= PLAYER_SPEED;
		}
	}

	al_destroy_display(display);

	return 0;
}
