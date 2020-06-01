#ifndef PR_CLIENTGAME_H
#define PR_CLIENTGAME_H


#include "allegro5/allegro_font.h"
#include "GameShared.h"


int nick_correction(char nick[]);
void draw_player(Player_t* player, ALLEGRO_FONT* font);
void draw_bullet(Bullet_t* bullet);
void draw_board(Board_t* board, ALLEGRO_FONT* font);
Board_t* start_game();


#endif