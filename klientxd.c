
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <allegro5\allegro.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_primitives.h>
#include <math.h>

#include <winsock2.h>
#include <windows.h>
#include <time.h>
#pragma comment(lib, "Ws2_32.lib")

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
ALLEGRO_FONT *font8;
const char* insertNick() {
	ALLEGRO_DISPLAY *okno = al_create_display(200, 80);
	al_set_window_title(okno, "Nick");
	font8 = al_create_builtin_font();

	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	al_register_event_source(queue, al_get_display_event_source(okno));
	al_register_event_source(queue, al_get_keyboard_event_source());
	ALLEGRO_EVENT event;

	const char* wprowadz = "Wprowadz nick";
	char* nick = (char*)malloc(NICK_LEN * sizeof(char));
	nick[0] = '\0';
	int nicklen = 0;

	al_draw_rectangle(45, 38, 160, 52, al_map_rgb(255, 255, 255), 2);
	al_draw_text(font8, al_map_rgb(255, 255, 255), 45, 20, 0, wprowadz);
	al_flip_display();

	bool running = true;
	while (running) {
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_text(font8, al_map_rgb(255, 255, 255), 50, 42, 0, nick);
		al_draw_rectangle(45, 38, 160, 52, al_map_rgb(255, 255, 255), 2);
		al_draw_text(font8, al_map_rgb(255, 255, 255), 45, 20, 0, wprowadz);

		al_wait_for_event(queue, &event);
		if (event.type == ALLEGRO_EVENT_KEY_DOWN) {

			if (event.keyboard.keycode >= ALLEGRO_KEY_A && event.keyboard.keycode <= ALLEGRO_KEY_Z) {
				nick[nicklen] = char(event.keyboard.keycode - 1 + 'A');
				nicklen += 1;
				nick[nicklen] = '\0';
			}
			else if (event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE) {
				nicklen -= 1;
				nick[nicklen] = '\0';
			}
			else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER)
				running = false;
		}
		if (nicklen > 13)
			running = false;
		al_flip_display();
	}

	al_destroy_display(okno);
	return nick;
}

//funkcyjka do narysowania jednego gracza
//do zrobienia rysowanie nicku
//KLIENT
int nickCorrection(char nick[]) {
	int ctr = 0;
	while (nick[ctr] != '\0')
		ctr++;
	return FONT_SIZE / (2.3)*ctr / 2;
}

void drawPlayer(Player* player) {
	//zamiana inta na stringa
	char scoreConverted[10];
	sprintf(scoreConverted, "%d", player->score);
	//rysowanie koła
	al_draw_filled_circle(player->posiotion.x, player->posiotion.y, PLAYER_SIZE, player->color);
	al_draw_text(font8, al_map_rgb(0, 0, 0), player->posiotion.x - nickCorrection(player->nick), player->posiotion.y + PLAYER_SIZE + 5, 0, player->nick);
	al_draw_text(font8, al_map_rgb(0, 0, 0), player->posiotion.x - nickCorrection(scoreConverted), player->posiotion.y + PLAYER_SIZE + 30, 0, scoreConverted);
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


typedef struct {
	char* nick;
	ALLEGRO_EVENT event;
} Message;
typedef  struct {
	char* nick;
	ALLEGRO_EVENT_QUEUE* queue;
} Klient_dane;


void sendMessage(Message message);

DWORD WINAPI wysylanie(void* args) {
	Klient_dane* dane = (Klient_dane*)args;
	ALLEGRO_EVENT_QUEUE* queue = dane->queue;
	char* nick = dane->nick;

	bool running = true;
	while (running) {
		ALLEGRO_EVENT event;

		al_wait_for_event(queue, &event);

		if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.type == ALLEGRO_KEY_ESCAPE)
			running = false;

		if (event.type == ALLEGRO_EVENT_MOUSE_AXES || event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN || event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_KEY_UP) {
			Message message = { nick, event };
			sendMessage(message);
		}
	}
}
