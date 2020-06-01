#ifndef PR_GAME_H
#define PR_GAME_H


#include <allegro5/allegro.h>
#include "List.h"


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

#define SCOREBOARD_SIZE_X 100

#define POINTS_FOR_KILL 10
#define BULLET_DAMAGE 0.25


typedef struct Coord {
	float x;
	float y;
} Cord;


typedef struct Player{
	char nick[NICK_LEN];
	int score;
	Cord position;
	float angle;
	ALLEGRO_COLOR color;
	float health;
}Player;


typedef struct Bullet{
	Cord position;
	float angle;
	Player* shooter;
}Bullet;


typedef struct Board{
	ListHandle_t* players;
	ListHandle_t* bullets;
}Board;


typedef struct BoardPacked{
	Player playersPacked;
	BulletPacked_t bulletsPacked;
	unsigned bytes;
}BoardPacked_t;


Board* init_board();
Bullet* init_bullet(Cord position, float angle, Player* shooter);
Player* init_player(const char* nick, Cord position, ALLEGRO_COLOR color);
BoardPacked_t* pack_board(Board* board);


#endif