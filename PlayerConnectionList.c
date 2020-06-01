#include <stdlib.h>
#include "List.h"
#include "ServerConnection.h"
#include "PlayerList.h"


ListElem_t* find_elem_pcn(ListHandle_t* list, SrvConnInfo_t* playerConn) {
	return  find_elem(list, (void*)playerConn);
}

ListElem_t* insert_beginning_pcn(ListHandle_t* list, SrvConnInfo_t* playerConn) {
	return insert_beginning(list, (void*)playerConn);
}


ListElem_t* insert_end_pcn(ListHandle_t* list, SrvConnInfo_t* playerConn) {
	return insert_end(list, (void*)playerConn);
}


void remove_pcn(ListHandle_t* list, SrvConnInfo_t* playerConn) {
	remove_elem(list, find_elem_pcn(list, playerConn));
}


int data_comparator_pcn(void* firstplayerConnData, void* secondplayerConnData){
	SrvConnInfo_t* frstPcn = (SrvConnInfo_t*)firstplayerConnData;
	SrvConnInfo_t* scndPcn = (SrvConnInfo_t*)secondplayerConnData;
	if (frstPcn->socket == scndPcn->socket) {
		return 0;
	}
	else {
		return 1;
	}
}


ListHandle_t* init_list_handle_pcn() {
	return init_list_handle(free, data_comparator_pcn);
}