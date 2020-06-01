#ifndef PR_PLAYERCONNLIST_H
#define PR_PLAYERCONNLIST_H


#include "List.h"
#include "ServerConnection.h"


ListElem_t* find_elem_pcn(ListHandle_t* list, SrvConnInfo_t* playerConn);
ListElem_t* insert_beginning_pcn(ListHandle_t* list, SrvConnInfo_t* playerConn);
ListElem_t* insert_end_pcn(ListHandle_t* list, SrvConnInfo_t* playerConn);
void remove_pcn(ListHandle_t* list, SrvConnInfo_t* playerConn);
int data_comparator_pcn(void* firstplayerConnData, void* secondplayerConnData);
ListHandle_t* init_list_handle_pcn();


#endif