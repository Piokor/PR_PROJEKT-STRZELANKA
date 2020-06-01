#include "Game.h"
#include "ServerGame.h"


void updateBullets(Board* board) {
	float x, y;
	ListElem_t* oldElem, *bulletElem = (board->bullets)->head;
	Bullet* bullet = NULL;

	while (bulletElem != NULL) {
		bullet = (Bullet*)(bulletElem->data);
		x = sin(bullet->angle) * BULLET_SPEED;
		y = cos(bullet->angle) * BULLET_SPEED;
		bullet->position.x += x;
		bullet->position.y += y;
		oldElem = bulletElem;
		bulletElem = bulletElem->next;
		if (bullet->position.x < 0 || bullet->position.y < 0 || bullet->position.x > BOARD_SIZE_X || bullet->position.y > BOARD_SIZE_Y) {
			remove_elem(board->bullets, oldElem);
		}
	}
}

//SERVER
void destroyPlayer(int i, Board* board) {
	board->playersAmt--;
	free(board->players[i]);
	if (i != board->playersAmt)
		board->players[i] = board->players[board->playersAmt];
}

//SERVER
void checkColisions(Board* board) {
	for (int ip = 0; ip < board->playersAmt; ip++) {
		for (int ib = 0; ib < board->bulletAmt; ib++) {
			float d = sqrt(pow(board->bullets[ib]->posiotion.x - board->players[ip]->posiotion.x, 2) + pow(board->bullets[ib]->posiotion.y - board->players[ip]->posiotion.y, 2)); //odleglosc gracza ip od pocisku ib
			if (d < PLAYER_SIZE + BULLET_SIZE) {
				board->players[ip]->health -= BULLET_DAMAGE;
				if (board->players[ip]->health <= 0) {
					board->bullets[ib]->shooter->score += POINTS_FOR_KILL;
					destroyBullet(ib, board);
					destroyPlayer(ip, board);
					ip = 0;
					ib = 0; //trzeba przeleciec od nowa, bo gracze i naboje mogli zmienic swoje polozenie w tablicy :<
				}
				else
					destroyBullet(ib, board);
			}
		}
	}
}