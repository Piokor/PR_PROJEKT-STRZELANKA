#ifndef PR_PLAYERLIST_H
#define PR_PLAYERIST_H


#include "List.h"


ListElem_t* find_elem_player(ListHandle_t* list, Player* player);
Player* find_player(ListHandle_t* list, Player* player);
ListElem_t* insert_beginning_player(ListHandle_t* list, Player* player);
ListElem_t* insert_end_player(ListHandle_t* list, Player* player);
void remove_player(ListHandle_t* list, Player* player);
int data_comparator_player(void* firstPlayerData, void* secondPlayerData);
ListHandle_t* init_list_handle_player();


#endif