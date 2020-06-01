#ifndef PR_BULLETLIST_H
#define PR_BULLETLIST_H


#include "List.h"


ListElem_t* find_elem_bullet(ListHandle_t* list, Bullet* bullet);
Bullet* find_bullet(ListHandle_t* list, Bullet* bullet);
ListElem_t* insert_beginning_bullet(ListHandle_t* list, Bullet* bullet);
ListElem_t* insert_end_bullet(ListHandle_t* list, Bullet* bullet);
void remove_bullet(ListHandle_t* list, Bullet* bullet);
ListHandle_t* init_list_handle_bullet();


#endif