#include "Game.h"
#include "BulletList.h"
#include "PlayerList.h"


Board* init_board() {
	Board* board = (Board*)malloc(sizeof(Board));
	board->players = init_list_handle_player();
	board->bullets = init_list_handle_bullet();
	return board;
}