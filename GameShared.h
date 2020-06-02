#ifndef PR_GAMESHARED_H
#define PR_GAMESHARED_H

#include <allegro5/allegro_color.h>
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

#define BOARD_SIZE_X 600
#define BOARD_SIZE_Y 600

#define SCOREBOARD_SIZE_X 100

#define AMT_OF_REPSAWN_CORDS 8
#define IMMORTAL_TIME 2

#define POINTS_FOR_KILL 10
#define BULLET_DAMAGE 0.25

#define FONT_SIZE 20
#define FPS 60


struct ALLEGRO_COLOR;


typedef struct Coord {
	float x;
	float y;
} Cord;


typedef struct Player{
	char nick[NICK_LEN];
	int score;
	Cord position;
	float angle;
	struct ALLEGRO_COLOR color;
	float health;
	double immortalTime;
	char* keys;
}Player_t;


typedef struct Bullet{
	Cord position;
	float angle;
	Player_t* shooter;
}Bullet_t;


typedef struct BulletPacked {
	Cord position;
	float angle;
	struct Player shooter;
}BulletPacked_t;


typedef struct Board{
	ListHandle_t* players;
	ListHandle_t* bullets;
}Board_t;


typedef struct BoardPacked{
	unsigned bytes[2];
	char* data;
}BoardPacked_t;


Board_t* init_board();
void free_board(Board_t* board);
Bullet_t* init_bullet(Cord position, float angle, Player_t* shooter);
Player_t* init_player(const char* nick, Cord position, ALLEGRO_COLOR color);


static Cord spawnCords[AMT_OF_REPSAWN_CORDS] = {
										{PLAYER_SIZE + BOARD_SIZE_X / 10, PLAYER_SIZE + BOARD_SIZE_Y / 10},
										{BOARD_SIZE_X / 2, PLAYER_SIZE + BOARD_SIZE_Y / 10},
										{BOARD_SIZE_X - PLAYER_SIZE - BOARD_SIZE_X / 10, PLAYER_SIZE + BOARD_SIZE_Y / 10},
										{PLAYER_SIZE + BOARD_SIZE_X / 10, BOARD_SIZE_Y / 2},
										{BOARD_SIZE_X - PLAYER_SIZE - BOARD_SIZE_X / 10, BOARD_SIZE_Y / 2},
										{PLAYER_SIZE + BOARD_SIZE_X / 10, BOARD_SIZE_Y - PLAYER_SIZE - BOARD_SIZE_Y / 10},
										{BOARD_SIZE_X / 2, BOARD_SIZE_Y - PLAYER_SIZE - BOARD_SIZE_Y / 10},
										{BOARD_SIZE_X - PLAYER_SIZE - BOARD_SIZE_X / 10, BOARD_SIZE_Y - PLAYER_SIZE - BOARD_SIZE_Y / 10}
};


#endif