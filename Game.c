#include <string.h>
#include "Game.h"
#include "BulletList.h"
#include "PlayerList.h"


Board* init_board() {
	Board* board = (Board*)malloc(sizeof(Board));

	board->players = init_list_handle_player();
	board->bullets = init_list_handle_bullet();

	return board;
}


Bullet* init_bullet(Cord position, float angle, Player* shooter) {
	Bullet* b = (Bullet*)malloc(sizeof(Bullet));
	b->position = position;
	b->angle = angle;
	b->shooter = shooter;
	return b;
}


Player* init_player(const char* nick, Cord position, ALLEGRO_COLOR color) {
	Player* p = (Player*)calloc(1, sizeof(Player));  // inicjuje zerami - nie trzeba ustawiaæ score i angle na zero

	int i = 0;
	char c = nick[0];
	while (c != '\0' && i != NICK_LEN) {
		p->nick[i++] = nick[i];
		c = nick[i];
	}

	p->position = position;
	p->color = color;
	p->health = 1;

	return p;
}


BoardPacked_t* pack_board(Board* board) {
	BoardPacked_t* boardPacked = (BoardPacked_t*)malloc(sizeof(BoardPacked_t));
	BulletPacked_t* packedBullets = pack_bullet_list(board->bullets);
	Player* packedPlayers = pack_player_list(board->players);

	boardPacked->bulletsPacked = *packedBullets;
	boardPacked->playersPacked = *packedPlayers;
	boardPacked->bytes = sizeof(boardPacked);

	free(packedBullets);
	free(packedPlayers);
	return boardPacked;
}