#include <math.h>
#include <Winsock2.h>
#include "BulletList.h"
#include "PlayerList.h"
#include "GameShared.h"
#include "ServerGame.h"


void update_bullets(Board_t* board) {
	if (board->bullets->isAllocated != 0) {
		float x, y;
		ListElem_t* bulletElem = board->bullets->head;
		Bullet_t* bullet = NULL;

		while (bulletElem != NULL) {
			bullet = (Bullet_t*)(bulletElem->data);
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
}


void respawn_player(Player_t* player) {
	player->health = 1.0;
	int newSpawn = rand() % AMT_OF_REPSAWN_CORDS;
	player->position.x = spawnCords[newSpawn].x;
	player->position.y = spawnCords[newSpawn].y;
	player->immortalTime = IMMORTAL_TIME;
}


void check_colisions(Board_t* board) {
	ListElem_t* bulletElem, *playerElem = board->players->head;
	Bullet_t* bullet = NULL;
	Player_t* player = NULL;
	char playerDead = 0;
	float distance = 0.0;

	while (playerElem != NULL && board->players->isAllocated != 0) {
		player = (Player_t*)playerElem->data;
		playerDead = 0;
		bulletElem = board->bullets->head;
		while (bulletElem != NULL && board->bullets->isAllocated != 0) {
			bullet = (Bullet_t*)bulletElem->data;
			distance = sqrt(pow(bullet->position.x - player->position.x, 2) + pow(bullet->position.y - player->position.y, 2));  // sodleglosc gracza od pocisku
			if (distance < PLAYER_SIZE + BULLET_SIZE) {
				player->health -= BULLET_DAMAGE;
				if (player->health <= 0) {
					bullet->shooter->score += POINTS_FOR_KILL;
					respawn_player(player);
					playerDead = 1;
					break;
				}
				else {
					bulletElem = remove_elem(board->bullets, bulletElem);
				}
			}
			else {
				bulletElem = bulletElem->next;
			}
		}
		if (playerDead == 0) {
			playerElem = playerElem->next;
		}
	}
}


void shoot(Player_t* shooter, Board_t* board){
	Bullet_t* bullet = init_bullet(shooter->position, M_PI - shooter->angle, shooter);

	bullet->position.x += sin(bullet->angle) * (PLAYER_SIZE + BULLET_SIZE + 1);
	bullet->position.y += cos(bullet->angle) * (PLAYER_SIZE + BULLET_SIZE + 1);

	insert_end(board->bullets, (void*)bullet);
}


SrvGameData_t* init_srv_game_data() {
	SrvGameData_t* sGD = (SrvGameData_t*)malloc(sizeof(SrvGameData_t));

	sGD->boardMutex = CreateMutex(NULL, FALSE, NULL);
	sGD->srvBoard = init_board();

	return sGD;
}


void update_immortal_timers(Board_t* board) {
	if (board->players->isAllocated != 0) {
		ListElem_t* currElem = board->players->head;
		Player_t* player = NULL;
		while (currElem != NULL) {
			player = (Player_t*)(currElem->data);
			if (player->immortalTime > 0) {
				player->immortalTime -= (1.0 / FPS);
			}
			else if (player->immortalTime < 0) {
				player->immortalTime = 0;
			}
		}
	}
}


BoardPacked_t* pack_board(Board_t* board) {
	BoardPacked_t* boardPacked = (BoardPacked_t*)malloc(sizeof(BoardPacked_t));
	BulletPacked_t* packedBullets = pack_bullet_list(board->bullets);
	Player_t* packedPlayers = pack_player_list(board->players);

	(boardPacked->bytes)[0] = sizeof(Player_t) * board->players->elementAmount;
	(boardPacked->bytes)[1] = sizeof(Bullet_t) * board->bullets->elementAmount;
	boardPacked->data = (char*)malloc((boardPacked->bytes)[0] + (boardPacked->bytes)[1]);
	memcpy(boardPacked->data, packedPlayers, (boardPacked->bytes)[0]);
	memcpy(boardPacked->data + (boardPacked->bytes)[0], packedBullets, (boardPacked->bytes)[1]);

	free(packedBullets);
	free(packedPlayers);
	return boardPacked;
}


void parse_event(ALLEGRO_EVENT* event, Board_t* board, char* nick) {
	Player_t* player = find_player_by_nick(board->players, nick);

	if (event->type == ALLEGRO_EVENT_MOUSE_AXES) {
		float x = event->mouse.x - player->position.x;
		float y = event->mouse.y - player->position.y;
		float poprawka = M_PI;
		if (x >= 0) poprawka = 0;
		float angle = atan(y / x) + M_PI / 2 + poprawka;
		player->angle = angle;

	}
	else if (event->type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && player->immortalTime == 0) {
		shoot(player, board);
	}
	else if (event->type == ALLEGRO_EVENT_KEY_DOWN) {
		switch (event->keyboard.keycode) {
		case ALLEGRO_KEY_UP:
			player->keys[UP] = 1;
			break;
		case ALLEGRO_KEY_DOWN:
			player->keys[DOWN] = 1;
			break;
		case ALLEGRO_KEY_LEFT:
			player->keys[LEFT] = 1;
			break;
		case ALLEGRO_KEY_RIGHT:
			player->keys[RIGHT] = 1;
			break;
		}
	}
	else if (event->type == ALLEGRO_EVENT_KEY_UP) {
		switch (event->keyboard.keycode) {
		case ALLEGRO_KEY_UP:
			player->keys[UP] = 0;
			break;
		case ALLEGRO_KEY_DOWN:
			player->keys[DOWN] = 0;
			break;
		case ALLEGRO_KEY_LEFT:
			player->keys[LEFT] = 0;
			break;
		case ALLEGRO_KEY_RIGHT:
			player->keys[RIGHT] = 0;
			break;
		}
	}
}