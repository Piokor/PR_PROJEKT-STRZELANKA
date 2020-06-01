#include <stdlib.h>
#include "List.h"


ListElem_t* allocate_elem(ListElem_t* prev, ListElem_t* next, void* data) {
	ListElem_t* newElem = (ListElem_t*)malloc(sizeof(ListElem_t));
	
	newElem->prev = prev;
	newElem->data = data;
	newElem->next = next;

	return newElem;
}


ListElem_t* find_elem(ListHandle_t* list, void* dataToFind){
	ListElem_t* currElem = NULL;

	if (list->isAllocated != 0) {
		currElem = list->head;
		while ((list->dataComparator)(dataToFind, currElem->data) != 0) {
			if(currElem->next == NULL){
				currElem = NULL;
				break;
			}
			currElem = currElem->next;
		}
	}
	
	return currElem;
}


void free_elem(ListHandle_t* list, ListElem_t* elem) {
	(list->dataFreer)(elem->data);
	free(elem);
}


ListHandle_t* remove_head(ListHandle_t* list) {
	if (list->isAllocated != 0) {
		free_elem(list, list->head);
		list->isAllocated = 0;
	}
	return list;
}


ListElem_t* insert_head(ListHandle_t* list, void* data) {
	list->head = allocate_elem(NULL, NULL, data);
	list->isAllocated = 1;
	return list->head;
}


ListElem_t* insert_beginning(ListHandle_t* list, void* data) {
	ListElem_t *oldHead, *newHead;
	oldHead = list->head;
	newHead = insert_head(list, data);

	if (list->isAllocated != 0) {
		oldHead->prev = newHead;
		newHead->next = oldHead;
	}
	list->elementAmount += 1;

	return newHead;
}


ListElem_t* insert_end(ListHandle_t* list, void* data) {
	ListElem_t* currElem = list->head;

	if (list->isAllocated == 0) {
		currElem = insert_head(list, data);
	}
	else {
		while (currElem->next != NULL)
			currElem = currElem->next;

		currElem->next = allocate_elem(currElem, NULL, data);
		currElem = currElem->next;
	}
	list->elementAmount += 1;
	
	return currElem;
}


ListElem_t* remove_elem(ListHandle_t* list, ListElem_t* elem) {
	ListElem_t* nextElem = elem->next;

	if (list->head == elem) {
		remove_head(list)->head;
	}
	else {
		elem->prev->next = nextElem;
		if (elem->next != NULL) {
			nextElem->prev = elem->prev;
		}
		free_elem(list, elem);
	}
	list->elementAmount -= 1;

	return nextElem;
}


ListHandle_t* init_list_handle(void(*dataFreer)(void*), int(*dataComparator)(void*, void*), unsigned dataSize) {
	ListHandle_t* handle = (ListHandle_t*)malloc(sizeof(ListHandle_t));
	handle->dataComparator = dataComparator;
	handle->dataFreer = dataFreer;
	handle->elementAmount = 0;
	handle->isAllocated = 0;

	return handle;
}