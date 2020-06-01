#ifndef PR_BULLETLIST_H
#define PR_BULLETLIST_H


#include "List.h"
#include "Game.h"


ListElem_t* find_elem_bullet(ListHandle_t* list, Bullet_t* bullet);
ListElem_t* insert_beginning_bullet(ListHandle_t* list, Bullet_t* bullet);
ListElem_t* insert_end_bullet(ListHandle_t* list, Bullet_t* bullet);
void remove_bullet(ListHandle_t* list, Bullet_t* bullet);
BulletPacked_t* pack_bullet_list(ListHandle_t* list);
ListHandle_t* init_list_handle_bullet();


#endif