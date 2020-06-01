#include <stdlib.h>
#include "List.h"
#include "Game.h"
#include "PlayerList.h"


ListElem_t* find_elem_player(ListHandle_t* list, Player_t* player) {
	return  find_elem(list, (void*)player);
}

ListElem_t* insert_beginning_player(ListHandle_t* list, Player_t* player) {
	return insert_beginning(list, (void*)player);
}


ListElem_t* insert_end_player(ListHandle_t* list, Player_t* player) {
	return insert_end(list, (void*)player);
}


void remove_player(ListHandle_t* list, Player_t* player) {
	remove_elem(list, find_elem_player(list, player));
}


int data_comparator_player(void* firstPlayerData, void* secondPlayerData){
	Player_t* frstPlay = (Player_t*)firstPlayerData;
	Player_t* scndPlay = (Player_t*)secondPlayerData;
	return strcmp(frstPlay->nick, scndPlay->nick);
}


ListHandle_t* init_list_handle_player() {
	return init_list_handle(free, data_comparator_player);
}


Player_t* pack_player_list(ListHandle_t* list) {
	if (list->isAllocated != 0) {
		Player_t* playersPacked = (Player_t*)malloc(sizeof(Player_t) * list->elementAmount);
		ListElem_t* currElem = list->head;
		unsigned playerNum = 0;
		
		while (currElem != NULL) {
			playersPacked[playerNum++] = *((Player_t*)(currElem->data));
			currElem = currElem->next;
		}

		return playersPacked;
	}
	else {
		return NULL;
	}
}