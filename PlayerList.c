#include <stdlib.h>
#include "List.h"
#include "Game.h"
#include "PlayerList.h"


ListElem_t* find_elem_player(ListHandle_t* list, Player* player) {
	return  find_elem(list, (void*)player);
}

ListElem_t* insert_beginning_player(ListHandle_t* list, Player* player) {
	return insert_beginning(list, (void*)player);
}


ListElem_t* insert_end_player(ListHandle_t* list, Player* player) {
	return insert_end(list, (void*)player);
}


void remove_player(ListHandle_t* list, Player* player) {
	remove_elem(list, find_elem_player(list, player));
}


int data_comparator_player(void* firstPlayerData, void* secondPlayerData){
	Player* frstPlay = (Player*)firstPlayerData;
	Player* scndPlay = (Player*)secondPlayerData;
	return strcmp(frstPlay->nick, scndPlay->nick);
}


ListHandle_t* init_list_handle_player() {
	return init_list_handle(free, data_comparator_player);
}


Player* pack_player_list(ListHandle_t* list) {
	if (list->isAllocated != 0) {
		Player* playersPacked = (Player*)malloc(sizeof(Player) * list->elementAmount);
		ListElem_t* currElem = list->head;
		unsigned playerNum = 0;
		
		while (currElem != NULL) {
			playersPacked[playerNum++] = *((Player*)(currElem->data));
			currElem = currElem->next;
		}

		return playersPacked;
	}
	else {
		return NULL;
	}
}