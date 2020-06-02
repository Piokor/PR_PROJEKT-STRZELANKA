#ifndef PR_PLAYERLIST_H
#define PR_PLAYERLIST_H


#include "List.h"


ListElem_t* find_elem_player(ListHandle_t* list, Player_t* player);
ListElem_t* insert_beginning_player(ListHandle_t* list, Player_t* player);
ListElem_t* insert_end_player(ListHandle_t* list, Player_t* player);
void remove_player(ListHandle_t* list, Player_t* player);
int data_comparator_player(void* firstPlayerData, void* secondPlayerData);
Player_t* pack_player_list(ListHandle_t* list);
Player_t* find_player_by_nick(ListHandle_t* list, char* nick);
ListHandle_t* init_list_handle_player();


#endif