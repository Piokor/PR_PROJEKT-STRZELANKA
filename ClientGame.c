#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include "ClientGame.h"
#include "PlayerList.h"
#include "BulletList.h"


int nick_correction(char nick[]) {
	int ctr = 0;
	while (nick[ctr] != '\0')
		ctr++;
	return FONT_SIZE / (2.3)*ctr / 2;
}


void draw_player(Player_t* player, ALLEGRO_FONT* font) {
	//rysowanie ko³a
	al_draw_filled_circle(player->position.x, player->position.y, PLAYER_SIZE, player->color);
	//zamiana inta na stringa
	char scoreConverted[10];
	sprintf(scoreConverted, "%d", player->score);
	//rysowanie nicku
	al_draw_text(font, al_map_rgb(0, 0, 0), player->position.x - nick_correction(player->nick), player->position.y + PLAYER_SIZE + 5, 0, player->nick);
	al_draw_text(font, al_map_rgb(0, 0, 0), player->position.x - nick_correction(scoreConverted), player->position.y + PLAYER_SIZE + 30, 0, scoreConverted);
	//twarda matma here - rysowanie strzaleczki
	al_draw_filled_triangle(player->position.x + (PLAYER_SIZE / 5) * sin(-player->angle), player->position.y + (PLAYER_SIZE / 5) * cos(-player->angle),
		player->position.x + (PLAYER_SIZE * 6 / 7) * sin(-player->angle + M_PI * 45 / 180), player->position.y + (PLAYER_SIZE * 6 / 7) * cos(-player->angle + M_PI * 45 / 180),
		player->position.x - (PLAYER_SIZE * 4 / 5) * sin(player->angle + M_PI), player->position.y + (PLAYER_SIZE * 4 / 5) * cos(player->angle + M_PI), al_map_rgb(0, 0, 0));
	al_draw_filled_triangle(player->position.x + (PLAYER_SIZE / 5) * sin(-player->angle), player->position.y + (PLAYER_SIZE / 5) * cos(-player->angle),
		player->position.x - (PLAYER_SIZE * 6 / 7) * sin(player->angle + M_PI * 45 / 180), player->position.y + (PLAYER_SIZE * 6 / 7) * cos(player->angle + M_PI * 45 / 180),
		player->position.x - (PLAYER_SIZE * 4 / 5) * sin(player->angle + M_PI), player->position.y + (PLAYER_SIZE * 4 / 5) * cos(player->angle + M_PI), al_map_rgb(0, 0, 0));
	//rysowanie
	float hpbarW = 28;
	float hpbarH = 4;
	al_draw_filled_rectangle(player->position.x - hpbarW / 2, player->position.y - PLAYER_SIZE - 5 - hpbarH, player->position.x + hpbarW / 2, player->position.y - PLAYER_SIZE - 5, al_map_rgb(0, 0, 0));
	al_draw_filled_rectangle(player->position.x - hpbarW / 2, player->position.y - PLAYER_SIZE - 5 - hpbarH, player->position.x - hpbarW / 2 + hpbarW * player->health, player->position.y - PLAYER_SIZE - 5, al_map_rgb(0, 200, 0));

}


void draw_bullet(Bullet_t* bullet) {
	al_draw_filled_circle(bullet->position.x, bullet->position.y, BULLET_SIZE, al_map_rgb(0, 0, 0));
}


void draw_board(Board_t* board, ALLEGRO_FONT* font) {
	ListElem_t* currElem;
	al_clear_to_color(al_map_rgb(20, 80, 150));

	if (board->players->isAllocated != 0) {
		currElem = board->players->head;
		while (currElem != NULL) {
			draw_player((Player_t*)(currElem->data), font);
			currElem = currElem->next;
		}
	}

	if (board->bullets->isAllocated != 0) {
		currElem = board->bullets->head;
		while (currElem != NULL) {
			draw_bullet((Bullet_t*)(currElem->data));
			currElem = currElem->next;
		}
	}

	al_flip_display();
}


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


Board_t* unpack_board(char* data, unsigned bulletsBytes, unsigned playersBytes) {
	Board_t* board = init_board();
	Player_t* playersData = (Player_t*)data;
	Bullet_t* bulletsData = (Bullet_t*)(data + playersBytes);

	while (playersBytes) {
		insert_beginning_player(board->players, playersData);
		playersData += sizeof(Player_t);
		playersBytes -= sizeof(Player_t);
	}

	while (bulletsBytes) {
		insert_beginning_bullet(board->bullets, bulletsData);
		bulletsData += sizeof(Bullet_t);
		bulletsBytes -= sizeof(Bullet_t);
	}

	return board;
}