#ifndef PR_LIST_H
#define PR_LIST_H


typedef struct ListElem {
	struct ListElem* prev;
	void* data;
	struct ListElem* next;
}ListElem_t;


typedef struct ListHandle {
	ListElem_t* head;
	char isAllocated;
	unsigned elementAmount;
	void (*dataFreer)(void*);
	int (*dataComparator)(void*, void*);
}ListHandle_t;



ListElem_t* allocate_elem(ListElem_t* prev, ListElem_t* next, void* data);
ListElem_t* find_elem(ListHandle_t* list, void* dataToFind);
void free_elem(ListHandle_t* list, ListElem_t* elem);
ListHandle_t* remove_head(ListHandle_t* list);
ListElem_t* insert_head(ListHandle_t* list, void* data);
ListElem_t* insert_beginning(ListHandle_t* list, void* data);
ListElem_t* insert_end(ListHandle_t* list, void* data);
ListElem_t* remove_elem(ListHandle_t* list, ListElem_t* elem);
ListHandle_t* init_list_handle(void(*dataFreer)(void*), int(*dataComparator)(void*, void*));


#endif