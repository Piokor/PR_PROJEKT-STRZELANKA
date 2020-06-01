#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <allegro5\allegro.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_primitives.h>
#include <math.h>
#define M_PI 3.14159265358979323846

#define PLAYER_SIZE 30
#define PLAYER_SPEED 2
#define BULLET_SIZE 3
#define BULLET_SPEED 10

#define NICK_LEN 15

#define UP 0
#define DOWN 2
#define LEFT 3
#define RIGHT 1

#define PLAYERS_LIMIT 10
#define BULLETS_LIMIT 30

#define BOARD_SIZE_X 600
#define BOARD_SIZE_Y 600

#define AMT_OF_REPSAWN_CORDS 8
#define IMMORTAL_TIME 2

#define POINTS_FOR_KILL 10
#define BULLET_DAMAGE 0.25

#define FONT_SIZE 20

ALLEGRO_FONT *font;
const float FPS = 60;



typedef struct {
	float x;
	float y;
}Cord; Cord spawnCords[AMT_OF_REPSAWN_CORDS] = { {PLAYER_SIZE + BOARD_SIZE_X / 10,PLAYER_SIZE + BOARD_SIZE_Y / 10},{BOARD_SIZE_X / 2,PLAYER_SIZE + BOARD_SIZE_Y / 10},{BOARD_SIZE_X - PLAYER_SIZE - BOARD_SIZE_X / 10,PLAYER_SIZE + BOARD_SIZE_Y / 10},
								{PLAYER_SIZE + BOARD_SIZE_X / 10,BOARD_SIZE_Y / 2},{BOARD_SIZE_X - PLAYER_SIZE - BOARD_SIZE_X / 10,BOARD_SIZE_Y / 2},
								{PLAYER_SIZE + BOARD_SIZE_X / 10,BOARD_SIZE_Y - PLAYER_SIZE - BOARD_SIZE_Y / 10},{BOARD_SIZE_X / 2,BOARD_SIZE_Y - PLAYER_SIZE - BOARD_SIZE_Y / 10},{BOARD_SIZE_X - PLAYER_SIZE - BOARD_SIZE_X / 10,BOARD_SIZE_Y - PLAYER_SIZE - BOARD_SIZE_Y / 10} };

typedef struct {
	char nick[NICK_LEN];
	int score;
	Cord posiotion;
	float angle;
	ALLEGRO_COLOR color;
	float health;
	double immortalTime;
}Player;

typedef struct {
	Cord posiotion;
	float angle;
	Player* shooter;
}Bullet;

typedef struct {
	Player** players;
	int playersAmt;
	Bullet** bullets;
	int bulletAmt;
}Board;

//funkcyjka do narysowania jednego gracza
//do zrobienia rysowanie nicku
//KLIENT
int nickCorrection(char nick[]) {
	int ctr = 0;
	while (nick[ctr] != '\0')
		ctr++;
	return FONT_SIZE/(2.3)*ctr / 2;
}

void drawPlayer(Player* player) {
	//zamiana inta na stringa
	char *scoreConverted[10];
	sprintf(scoreConverted, "%d", player->score);
	//rysowanie koła
	al_draw_filled_circle(player->posiotion.x, player->posiotion.y, PLAYER_SIZE, player->color);
	al_draw_text(font, al_map_rgb(0, 0, 0), player->posiotion.x - nickCorrection(player->nick), player->posiotion.y + PLAYER_SIZE+5, 0, player->nick);
	al_draw_text(font, al_map_rgb(0, 0, 0), player->posiotion.x - nickCorrection(scoreConverted), player->posiotion.y + PLAYER_SIZE+30, 0, scoreConverted);
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
	p->immortalTime = 0;
	p->nick[i] = '\0';
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

//tutaj jest harcodowana ilosc graczy i pocisków mozliwych, moze do zmiany potem nie wiem

Board* initBoard() {
	Board* board = (Board*)malloc(sizeof(Board));
	board->players = (Player**)malloc(PLAYERS_LIMIT * sizeof(Player*));
	board->playersAmt = 0;
	board->bullets = (Bullet**)malloc(PLAYERS_LIMIT * sizeof(Bullet*));
	board->bulletAmt = 0;
	return board;
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

//SERVER
void destroyBullet(int i, Board* board) {
	board->bulletAmt--;
	free(board->bullets[i]);
	if (i != board->bulletAmt)
		board->bullets[i] = board->bullets[board->bulletAmt];
}

//SERVER 
void updateImmortalTimers(Board * board) {
	for (int i = 0; i < board->playersAmt; i++) {
		if (board->players[i]->immortalTime) {
			board->players[i]->immortalTime -= (1.0 / FPS);
		}
		if (board->players[i]->immortalTime<0) {
			board->players[i]->immortalTime = 0;
		}
	}
}


//SERVER
void updateBullets(Board* board) {
	for (int i = 0; i < board->bulletAmt; i++) {
		float x = sin(board->bullets[i]->angle) * BULLET_SPEED;
		float y = cos(board->bullets[i]->angle) * BULLET_SPEED;
		board->bullets[i]->posiotion.x += x;
		board->bullets[i]->posiotion.y += y;
		//usuniecie pocisku jesli wyjechal poza plansze
		if (board->bullets[i]->posiotion.x < 0 || board->bullets[i]->posiotion.y < 0 || board->bullets[i]->posiotion.x > BOARD_SIZE_X || board->bullets[i]->posiotion.y > BOARD_SIZE_Y) {
			destroyBullet(i, board);
		}
	}
}

//SERVER
void respawnPlayer(int i, Board *board) {
	board->players[i]->health = 1.0;
	int newSpawn = rand() % AMT_OF_REPSAWN_CORDS;
	board->players[i]->posiotion.x = spawnCords[newSpawn].x;
	board->players[i]->posiotion.y = spawnCords[newSpawn].y;
	board->players[i]->immortalTime = IMMORTAL_TIME;
}

//SERVER
void destroyPlayer(int i, Board* board) {
	board->playersAmt--;
	free(board->players[i]);
	if (i != board->playersAmt)
		board->players[i] = board->players[board->playersAmt];
}

//SERVER
void checkColisions(Board* board) {
	for (int ip = 0; ip < board->playersAmt; ip++) {
		for (int ib = 0; ib < board->bulletAmt; ib++) {
			float d = sqrt(pow(board->bullets[ib]->posiotion.x - board->players[ip]->posiotion.x, 2) + pow(board->bullets[ib]->posiotion.y - board->players[ip]->posiotion.y, 2)); //odleglosc gracza ip od pocisku ib
			if (d < PLAYER_SIZE + BULLET_SIZE && board->players[ip]->immortalTime==0) {
				board->players[ip]->health -= BULLET_DAMAGE;
				if (board->players[ip]->health <= 0) {
					board->bullets[ib]->shooter->score += POINTS_FOR_KILL;
					destroyBullet(ib, board);
					//destroyPlayer(ip, board); stare
					respawnPlayer(ip, board);
					ip = 0;
					ib = 0; //trzeba przeleciec od nowa, bo gracze i naboje mogli zmienic swoje polozenie w tablicy :<
				}
				else
					destroyBullet(ib, board);
			}
		}
	}
}


int main(int argc, char* argv[]) {

	//inicjalizacji w bród
	ALLEGRO_DISPLAY* display = NULL;
	srand(time(NULL));

	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();

	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	display = al_create_display(BOARD_SIZE_X, BOARD_SIZE_Y);
	if (!display) {
		fprintf(stderr, "failed to create display!\n");
		return -1;
	};


	font = al_load_font("data/arial.ttf", FONT_SIZE, NULL);

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
	Player* mainPlayer = initPlayer("Nikodem", 0, spawnCords[rand()%AMT_OF_REPSAWN_CORDS], 0, al_map_rgb(100, 0, 0));
	Player* debil = initPlayer("Jakub", 0, spawnCords[rand() % AMT_OF_REPSAWN_CORDS], 0, al_map_rgb(0, 100, 0));
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
		else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && mainPlayer->immortalTime==0) {
			shoot(mainPlayer, board);
		}
		else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (event.keyboard.keycode) {
			case ALLEGRO_KEY_ESCAPE:
				running = false;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = 1;
				printf("UP");
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
			updateImmortalTimers(board);
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
