#include "BulletList.h"
#include "PlayerList.h"
#include "GameShared.h"


Board_t* init_board() {
	Board_t* board = (Board_t*)malloc(sizeof(Board_t));

	board->players = init_list_handle_player();
	board->bullets = init_list_handle_bullet();

	return board;
}


void free_board(Board_t* board) {
	remove_all_elems(board->bullets);
	remove_all_elems(board->players);
	free(board->bullets);
	free(board->players);
	free(board);
}


Bullet_t* init_bullet(Cord position, float angle, Player_t* shooter) {
	Bullet_t* b = (Bullet_t*)malloc(sizeof(Bullet_t));
	b->position = position;
	b->angle = angle;
	b->shooter = shooter;
	return b;
}


Player_t* init_player(const char* nick, Cord position, ALLEGRO_COLOR color) {
	Player_t* p = (Player_t*)calloc(1, sizeof(Player_t));  // inicjuje zerami - nie trzeba ustawia� score i angle na zero

	int i = 0;
	char c = nick[0];
	while (c != '\0' && i != NICK_LEN) {
		p->nick[i++] = nick[i];
		c = nick[i];
	}

	p->position = position;
	p->color = color;
	p->health = 1;
	p->immortalTime = 0;
	p->keys = (char*)calloc(4, sizeof(char));

	return p;
}