#ifndef PR_SERVERGAME_H
#define PR_SERVERGAME_H


#include "Game.h"


typedef struct SrvGameData {
	Board* srvBoard;
	void* boardMutex;
}SrvGameData_t;


void update_bullets(Board* board);
void check_colisions(Board* board);
void shoot(Player* shooter, Board* board);
SrvGameData_t* init_srv_game_data();


#endif