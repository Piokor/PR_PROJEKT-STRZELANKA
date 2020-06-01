#include <stdlib.h>
#include "List.h"
#include "Game.h"
#include "BulletList.h"


ListElem_t* find_elem_bullet(ListHandle_t* list, Bullet* bullet) {
	return  find_elem(list, (void*)bullet);
}

ListElem_t* insert_beginning_bullet(ListHandle_t* list, Bullet* bullet) {
	return insert_beginning(list, (void*)bullet);
}


ListElem_t* insert_end_bullet(ListHandle_t* list, Bullet* bullet) {
	return insert_end(list, (void*)bullet);
}


void remove_bullet(ListHandle_t* list, Bullet* bullet) {
	remove_elem(list, find_elem_bullet(list, bullet));
}


int data_comparator_bullet(void* firstBulletData, void* secondBulletData){
	Bullet* frstBull = (Bullet*)firstBulletData;
	Bullet* scndBull = (Bullet*)secondBulletData;
	if (frstBull->position.x == scndBull->position.x && frstBull->position.y == scndBull->position.y &&
		frstBull->shooter == scndBull->shooter) {
		return 0;
	}
	else {
		return 1;
	}
}


ListHandle_t* init_list_handle_bullet() {
	return init_list_handle(free, data_comparator_bullet);
}