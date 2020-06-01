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


BulletPacked_t* pack_bullet_list(ListHandle_t* list) {
	if (list->isAllocated != 0) {
		BulletPacked_t* bulletsPacked = (BulletPacked_t*)malloc(sizeof(BulletPacked_t) * list->elementAmount);
		Bullet* bullet = NULL;
		ListElem_t* currElem = list->head;
		unsigned bNum = 0;

		while (currElem != NULL) {
			bullet = (Bullet*)(currElem->data);
			bulletsPacked[bNum].angle = bullet->angle;
			bulletsPacked[bNum].position = bullet->position;
			bulletsPacked[bNum++].shooter = *(bullet->shooter);
			currElem = currElem->next;
		}

		return bulletsPacked;
	}
	else {
		return NULL;
	}
}