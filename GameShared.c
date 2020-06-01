#include "BulletList.h"
#include "PlayerList.h"
#include "GameShared.h"

Board_t* init_board() {
	Board_t* board = (Board_t*)malloc(sizeof(Board_t));

	board->players = init_list_handle_player();
	board->bullets = init_list_handle_bullet();

	return board;
}


Bullet_t* init_bullet(Cord position, float angle, Player_t* shooter) {
	Bullet_t* b = (Bullet_t*)malloc(sizeof(Bullet_t));
	b->position = position;
	b->angle = angle;
	b->shooter = shooter;
	return b;
}


Player_t* init_player(const char* nick, Cord position, ALLEGRO_COLOR color) {
	Player_t* p = (Player_t*)calloc(1, sizeof(Player_t));  // inicjuje zerami - nie trzeba ustawiaæ score i angle na zero

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


BoardPacked_t* pack_board(Board_t* board) {
	BoardPacked_t* boardPacked = (BoardPacked_t*)malloc(sizeof(BoardPacked_t));
	BulletPacked_t* packedBullets = pack_bullet_list(board->bullets);
	Player_t* packedPlayers = pack_player_list(board->players);

	boardPacked->bulletsPacked = *packedBullets;
	boardPacked->playersPacked = *packedPlayers;
	boardPacked->bytes = sizeof(boardPacked);

	free(packedBullets);
	free(packedPlayers);
	return boardPacked;
}