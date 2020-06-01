#ifndef PR_BULLETLIST_H
#define PR_BULLETLIST_H


#include "List.h"


struct Bullet;
struct Player;


typedef struct BulletPacked {
	Cord position;
	float angle;
	struct Player shooter;
}BulletPacked_t;


ListElem_t* find_elem_bullet(ListHandle_t* list, struct Bullet* bullet);
ListElem_t* insert_beginning_bullet(ListHandle_t* list, struct Bullet* bullet);
ListElem_t* insert_end_bullet(ListHandle_t* list, struct Bullet* bullet);
void remove_bullet(ListHandle_t* list, struct Bullet* bullet);
BulletPacked_t* pack_bullet_list(ListHandle_t* list);
ListHandle_t* init_list_handle_bullet();


#endif