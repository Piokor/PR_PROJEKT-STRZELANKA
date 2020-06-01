#ifndef PR_SERVERGAME_H
#define PR_SERVERGAME_H


#include "GameShared.h"


typedef struct SrvGameData {
	Board_t* srvBoard;
	void* boardMutex;
}SrvGameData_t;


void update_bullets(Board_t* board);
void check_colisions(Board_t* board);
void shoot(Player_t* shooter, Board_t* board);
SrvGameData_t* init_srv_game_data();
BoardPacked_t* pack_board(Board_t* board);
void update_immortal_timers(Board_t* board);


#endif