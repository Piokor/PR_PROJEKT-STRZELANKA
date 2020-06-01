#ifndef PR_SERVERGAME_H
#define PR_SERVERGAME_H


#include <WinSock2.h>
#include "Game.h"


typedef struct SrvGameData {
	Board* srvBoard;
	HANDLE boardMutex;
}SrvGameData_t;


Board* init_board();


#endif