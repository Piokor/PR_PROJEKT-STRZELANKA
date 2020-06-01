#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include "ClientGame.h"


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


void unpack_board() {

}