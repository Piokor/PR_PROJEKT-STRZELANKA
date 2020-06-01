#include "Game.h"
#include "ServerGame.h"


void update_bullets(Board* board) {
	float x, y;
	ListElem_t* bulletElem = board->bullets->head;
	Bullet* bullet = NULL;

	while (bulletElem != NULL) {
		bullet = (Bullet*)(bulletElem->data);
		x = sin(bullet->angle) * BULLET_SPEED;
		y = cos(bullet->angle) * BULLET_SPEED;
		bullet->position.x += x;
		bullet->position.y += y;
		if (bullet->position.x < 0 || bullet->position.y < 0 || bullet->position.x > BOARD_SIZE_X || bullet->position.y > BOARD_SIZE_Y) {
			bulletElem = remove_elem(board->bullets, bulletElem);
		}
		else {
			bulletElem = bulletElem->next;
		}
	}
}


void check_colisions(Board* board) {
	ListElem_t* bulletElem, *playerElem = board->players->head;
	Bullet* bullet = NULL;
	Player* player = NULL;
	float distance = 0.0;

	while (playerElem != NULL) {
		player = (Player*)playerElem->data;
		bulletElem = board->bullets->head;
		while (bulletElem != NULL) {
			bullet = (Bullet*)bulletElem->data;
			distance = sqrt(pow(bullet->position.x - player->position.x, 2) + pow(bullet->position.y - player->position.y, 2));  // sodleglosc gracza od pocisku
			if (distance < PLAYER_SIZE + BULLET_SIZE) {
				player->health -= BULLET_DAMAGE;
				if (player->health <= 0) {
					bullet->shooter->score += POINTS_FOR_KILL;
					playerElem = remove_elem(board->players, playerElem);
					break;
				}
				else {
					bulletElem = remove_elem(board->bullets, bulletElem);
				}
			}
		}
	}
}