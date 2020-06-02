#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <allegro5/allegro.h>
#include "ServerConnection.h"



int main(int argc, char* argv[]) {
	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	ShooterServer_t* srv = create_server();
	start_server(srv);

	return 0;
}

/*
ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
ALLEGRO_TIMER* timer = al_create_timer(0.001 * SRV_REFRESH_TIME_MS);
al_register_event_source(queue, al_get_timer_event_source(timer));
al_start_timer(timer);



if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
	float x = event.mouse.x - mainPlayer->position.x;
	float y = event.mouse.y - mainPlayer->position.y;
	float poprawka = M_PI;
	if (x >= 0) poprawka = 0;
	float angle = atan(y / x) + M_PI / 2 + poprawka; //no obliczenia cos nie wysz³y, ale jakos sie za³atalo
	mainPlayer->angle = angle;

}
else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
	shoot(mainPlayer, board);
}
else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
	switch (event.keyboard.keycode) {
	case ALLEGRO_KEY_ESCAPE:
		running = false;
		break;
	case ALLEGRO_KEY_UP:
		keys[UP] = 1;
		break;
	case ALLEGRO_KEY_DOWN:
		keys[DOWN] = 1;
		break;
	case ALLEGRO_KEY_LEFT:
		keys[LEFT] = 1;
		break;
	case ALLEGRO_KEY_RIGHT:
		keys[RIGHT] = 1;
		break;
	}
}
else if (event.type == ALLEGRO_EVENT_KEY_UP) {
	switch (event.keyboard.keycode) {
	case ALLEGRO_KEY_UP:
		keys[UP] = 0;
		break;
	case ALLEGRO_KEY_DOWN:
		keys[DOWN] = 0;
		break;
	case ALLEGRO_KEY_LEFT:
		keys[LEFT] = 0;
		break;
	case ALLEGRO_KEY_RIGHT:
		keys[RIGHT] = 0;
		break;
	}
}
if (event.type == ALLEGRO_EVENT_TIMER) {
	update_bullets(board);
	check_colisions(board);
	if (keys[LEFT])
		mainPlayer->position.x -= PLAYER_SPEED;
	if (keys[RIGHT])
		mainPlayer->position.x += PLAYER_SPEED;
	if (keys[DOWN])
		mainPlayer->position.y += PLAYER_SPEED;
	if (keys[UP])
		mainPlayer->position.y -= PLAYER_SPEED;
}*/