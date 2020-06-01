#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include <Winsock2.h>
#include "GameShared.h"
#include "ServerGame.h"
#include "BulletList.h"
#include "ClientGame.h"
#include "PlayerList.h"


/*typedef struct {
	char* nick;
	ALLEGRO_EVENT event;
} Message;

typedef  struct {
	char* nick;
	ALLEGRO_EVENT_QUEUE* queue;
} Klient_dane;


void sendMessage(Message message) {};

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
			Message message = (Message){ nick, event };
			sendMessage(message);
		}
	}
}*/






const char* insertNick() {
	ALLEGRO_FONT *font8 = al_create_builtin_font();
	ALLEGRO_DISPLAY *okno = al_create_display(200, 80);
	al_set_window_title(okno, "Nick");

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
				nick[nicklen] = (char)(event.keyboard.keycode - 1 + 'A');
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


const char* insertIP() {
	ALLEGRO_DISPLAY *okno = al_create_display(250, 80);
	al_set_window_title(okno, "IP");
	ALLEGRO_FONT* font8 = al_create_builtin_font();

	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	al_register_event_source(queue, al_get_display_event_source(okno));
	al_register_event_source(queue, al_get_keyboard_event_source());
	ALLEGRO_EVENT event;

	const char* wprowadz = "Wprowadz IP serwera";
	char* ip = (char*)malloc((16 + 3 + 1) * sizeof(char));
	ip[0] = '\0';
	int iplen = 0;

	al_draw_rectangle(45, 38, 210, 52, al_map_rgb(255, 255, 255), 2);
	al_draw_text(font8, al_map_rgb(255, 255, 255), 45, 20, 0, wprowadz);
	al_flip_display();

	bool running = true;
	while (running) {
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_text(font8, al_map_rgb(255, 255, 255), 50, 42, 0, ip);
		al_draw_rectangle(45, 38, 210, 52, al_map_rgb(255, 255, 255), 2);
		al_draw_text(font8, al_map_rgb(255, 255, 255), 45, 20, 0, wprowadz);

		al_wait_for_event(queue, &event);
		if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
			if (event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE) {
				iplen -= 1;
				ip[iplen] = '\0';
			}
			else if (event.keyboard.keycode >= ALLEGRO_KEY_0 && event.keyboard.keycode <= ALLEGRO_KEY_9) {
				if (iplen < 20) {
					ip[iplen] = (char)(event.keyboard.keycode - ALLEGRO_KEY_0 + '0');
					iplen += 1;
					ip[iplen] = '\0';
				}
			}
			else if (event.keyboard.keycode == ALLEGRO_KEY_FULLSTOP) {
				if (iplen < 20) {
					ip[iplen] = '.';
					iplen += 1;
					ip[iplen] = '\0';
				}
			}
			else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER)
				running = false;
		}
		al_flip_display();
	}

	al_destroy_display(okno);
	return ip;
}


int main(int argc, char* argv[]) {

	//inicjalizacji w bród
	ALLEGRO_DISPLAY* display = NULL;

	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();


	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	ALLEGRO_FONT* font = al_load_ttf_font("arial.ttf", FONT_SIZE, 0);
	al_install_keyboard();
	al_install_mouse();

	const char* nick = insertNick();
	const char* ip = insertIP();

	display = al_create_display(BOARD_SIZE_X, BOARD_SIZE_Y);
	if (!display) {
		fprintf(stderr, "failed to create display!\n");
		return -1;
	};

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);

	ALLEGRO_EVENT_QUEUE* queue;
	queue = al_create_event_queue();



	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_mouse_event_source());
	al_register_event_source(queue, al_get_timer_event_source(timer));

	al_start_timer(timer);
	//koniec inicjalizacji

	al_clear_to_color(al_map_rgb(20, 80, 150));

	Cord c = { 350, 350 };
	Player_t* mainPlayer = init_player(nick, c, al_map_rgb(100, 0, 0));
	c = (Cord){ 100, 100 };
	Player_t* debil = init_player("Jan", c, al_map_rgb(0, 100, 0));
	float angle = 0;
	bool running = true;
	bool keys[4] = { 0,0,0,0 };

	Board_t* board = init_board();
	insert_end(board->players, (void*)mainPlayer);
	insert_end(board->players, (void*)debil);

	while (running) {
		ALLEGRO_EVENT event;

		draw_board(board, font);
		al_wait_for_event(queue, &event);
		// to tutaj jest do zmiany - obliczenia beda wykonywane po stronie servera, a po kazdym evencie bedzie wysyłany sygnal
		// to jest wersja testowa w sumie
		if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
			float x = event.mouse.x - mainPlayer->position.x;
			float y = event.mouse.y - mainPlayer->position.y;
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
			update_bullets(board);
			check_colisions(board);
			if (keys[LEFT])
				mainPlayer->position.x -= PLAYER_SPEED;
			if (keys[RIGHT])
				mainPlayer->position.x += PLAYER_SPEED;
			if (keys[DOWN])
				mainPlayer->position.y += PLAYER_SPEED;
			if (keys[UP])
				mainPlayer->position.y -= PLAYER_SPEED;
		}
	}

	al_destroy_display(display);

	return 0;
}